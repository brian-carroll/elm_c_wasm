## TextDecoder performance benchmarks for UTF-8 and UTF-16

Decoding times in microseconds from JS [typed array](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Typed_arrays) to string, averaged over 600 iterations.

This represents the boundary crossing from WebAssembly to JS to interact with Web APIs. All Web APIs use JS strings, so this information informs which encoding Elm should use in Wasm.

Should we use UTF-8, or stick to JS-like strings (UTF-16 but with a wrong API that treats code points as characters).

Tested using ASCII and Chinese. There's no special reason to pick Chinese, it's just to represent some non-ASCII use case. Same file size for each (approx 60kB)

|                 | tDecode utf8 | tDecode utf16 |     | bytelength utf8 | bytelength utf16 |     | tDecode/kB utf8 | tDecode/kB utf16 |
| --------------- | ------------ | ------------- | --- | --------------- | ---------------- | --- | --------------- | ---------------- |
| Firefox Chinese | 230          | 68            |     | 61524           | 51076            |     | 3.83            | 1.36             |
| Firefox ASCII   | 87           | 127           |     | 61524           | 123048           |     | 1.45            | 1.06             |
| Chrome Chinese  | 297          | 174           |     | 61524           | 51076            |     | 4.94            | 3.49             |
| Chrome ASCII    | 117          | 441           |     | 61524           | 123048           |     | 1.94            | 3.67             |

The first set of numbers (decode time) are the measurements.
'Decode time per kilobyte' helps to analyse what's going on.

### Analysis

- For UTF-16, language doesn't matter much. For UTF-8, it matters a lot.
- UTF-16 is slower for the ASCII text, and it seems to be mostly explained by the larger bytelength.
- UTF-8 is slower for the Chinese text because there's more actual format conversion arithmetic to do.
- The Chinese text is smaller in UTF-16. Many Chinese characters are in the range 0800 to FFFF, which needs 3 bytes in UTF-8 but only 2 in UTF-16
- I'm surprised Chrome is slower per kB to decode UTF-16 to UTF-16 than UTF-8 to UTF-16. Maybe their code is somehow generic, not optimised for this case. Firefox seems more optimised
- If most of the text is ASCII (DOM node names and so on) then speed depends mainly on byte size. UTF-16 bloats the size, slowing things down. The fact that it's the same as native JS strings doesn't matter for ASCII range
- BUT if most of the text is content, then performance depends hugely on the language
- On Chrome, UTF-8 and UTF-16 should perform about the same if text is half ASCII and half Chinese ( (3.49+3.67)/2 is roughly the same as (1.94+4.94)/2 )

### Conclusions

- The speed benefit I thought would be there for UTF-16 doesn't seem to be as strong as I'd expected.
- My theory was that in a browser environment, it's obviously better to avoid converting formats. But it's not so obvious. The bloated bytesize counteracts that benefit, especially in ASCII range where there's no difference between the encodings.
- It's harder to argue against the 'industry standard' UTF-8 when UTF-16 doesn't have the overwhelming benefit I thought it might.

### Backward compatibility

- But using UTF-8 would change how `String.length` works, as well as lots of other String and Char functions. Would break existing apps, making it harder to port to Wasm and to test my compiler and kernel libs.
- This could turn into a Python v2/v3 problem. That 12-year mess was in large part to do with strings and Unicode as well.
- Maybe there need to be two versions of each of these libraries, make it a compile option. Maybe only a few basic functions are different like foldl, foldr. Build everything on top of that.
