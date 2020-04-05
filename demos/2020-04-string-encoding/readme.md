## TextDecoder performance benchmarks for UTF-8 and UTF-16

Averaged over 600 iterations per encoding
Exact same HTML file size in ASCII and Chinese, approx 60kB

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
- If most of the text is ASCII (DOM node names and so on) then speed depends mainly on byte size. UTF-16 bloats the size, slowing things down.
- The fact that it's the same as native JS strings doesn't matter for ASCII range
- BUT if most of the text is content, then performance depends hugely on the language
- On Chrome, UTF-8 and UTF-16 should perform about the same if text is half ASCII and half Chinese ( (3.49+3.67)/2 is roughly the same as (1.94+4.94)/2 )

### Conclusions

- The speed benefit I thought would be there for UTF-16 doesn't seem to be as clear.
- My theory was that in a browser environment, it's obviously better not to have to do a conversion. But it's not that obvious. If you're doing a copy anyway maybe conversion is not much extra overhead.
- But I wonder what the binary interface to the DOM is going to look like in future Wasm though! Will it also require the string to be copied? Where to put the TextDecoder?
- It's hard to argue against the 'industry standard' UTF-8 when UTF-16 doesn't have a huge benefit in performance

### Backward compatibility

- But using UTF-8 would change how String.length works, as well as lots of other String and Char functions. Would break existing apps.
- This could turn into a Python v2/v3 problem. That 12-year mess was in large part to do with strings and Unicode as well.
- Maybe there need to be two versions of each of these libraries, make it a compile option. Maybe only a few basic functions are different like foldl, foldr. Maybe length (though it's a fold)
