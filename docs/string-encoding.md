# String Encoding

WebAssembly has no string primitives, so they have to be implemented at the byte level. Different source languages targeting WebAssembly may have different string representations, and WebAssembly needs to support that.

The String struct defined in [types.h](/src/kernel/types.h) contains header and a sequence of bytes. There are various "encodings" of characters to bytes, and the modern _de facto_ standard is UTF-8. A lot of recently-developed languages use it as their default encoding (Go, Rust, etc.). But the best encoding to use also depends on what other systems you're interfacing to, and _Elm mainly interfaces to the Web APIs_. We'll get into all of this below.

## String representations in other languages

### JavaScript

[This article][js-string-encoding] gives a detailed description of JavaScript's string representation. The summary is that the ECMAScript standard allows for engines to use either UTF-16 or UCS-2, which are similar but slightly different. Most engines use UTF-16.

[js-string-encoding]: https://mathiasbynens.be/notes/javascript-encoding

### Python

The Python 3 Standard library has a Unicode type whose API is Unicode code points. But as per [PEP 393][pep-393], the underlying representation is a C structure uses a variety of storage formats (ASCII, latin-1, UCS-2, UCS-4, or UTF8). The choice depends on various factors including the maximum character value at creation time. The structure also holds metadata including length, a hash (for use in dictionary keys etc.), and a tag indicating which encoding is used.

[pep-393]: https://www.python.org/dev/peps/pep-0393/

### Java

Similarly, Java's [JEP 254][jep-254] describes multiple string representations depending on the maximum character value. However all the built-in representations use units of either 8 or 16 bits. There is no built-in Unicode support but there are libraries to support it. A detailed density analysis of different string types for Java can be found [here][java-string-density].

[jep-254]: http://openjdk.java.net/jeps/254
[java-string-density]: http://cr.openjdk.java.net/~shade/density/state-of-string-density-v1.txt

### Rust

Rust's [String][rust-string] consists of "a pointer to some bytes, a length, and a capacity. The pointer points to an internal buffer String uses to store its data. The length is the number of bytes currently stored in the buffer, and the capacity is the size of the buffer in bytes." The internal representation is UTF-8. There are APIs to convert between bytes and strings.

[rust-string]: https://doc.rust-lang.org/std/string/struct.String.html#representation

### OCaml

OCaml's [String][ocaml-string] library is based on a sequence of one-byte characters. Unicode support doesn't seem to be strong.

[ocaml-string]: https://caml.inria.fr/pub/docs/manual-ocaml/libref/String.html

### Summary

Most languages seem to grapple with a tradeoff between Unicode compliance, convenience, and memory density. It's considered best practice to present the application programmer with an API that treats strings as sequences of Unicode characters, while trying to have an underlying representation that is as dense as possible.

Most guides on this are targeted at application developers rather than language implementers. Best practice guides such as [The Unicode Book][unicode-book] and one from the [Flask][flask] web framework, advocate that programs should deal exclusively with Unicode characters internally, and only do encoding and decoding when dealing with external systems you can't control. In Elm this means the `String` package should provide functions to iterate over Unicode `Char`s and only Effect Managers should deal with encodings.

The internal memory representation should be something that facilitates this.

[flask]: http://flask.pocoo.org/docs/1.0/unicode/
[unicode-book]: https://unicodebook.readthedocs.io/good_practices.html

## Strings with Web APIs

Most of the browser's Web APIs use JavaScript Strings in UTF-16 format. For example `document.getElementById` expects its argument to be a [DOMString][domstring], which is UTF-16. `XmlHttpRequest` can deal with UTF-8 request and response bodies, but what about the string that specifies the URL? That's usually done with JavaScript strings. When the WebAssembly API comes out, will that require UTF-16 too? I can only suppose that the browser's underlying C++ implementation expects UTF-16, so wouldn't it present this to WebAssembly?

[domstring]: https://developer.mozilla.org/en-US/docs/Web/API/DOMString

There's limited information at this stage on how the Web APIs will work with WebAssembly. There's an [overview of the proposal][host-bindings] but it seems to leave a lot up to browser vendors. It focuses on very low-level details and doesn't say anything about specific APIs like DOM or HTTP.

[host-bindings]: https://github.com/WebAssembly/host-bindings/blob/master/proposals/host-bindings/Overview.md

The general idea is that each Web API will be represented as a "table" of numbered functions. To send a string from Wasm to a browser API, the Wasm program writes it to its own block of memory and passes the address and length to one of the API functions. The Wasm memory block is visible to JavaScript as an ArrayBuffer and also visible to browser APIs, so it can be read from there.

When the browser sends a string to Wasm, calls an "exported" function in the Wasm program to tell it how much memory to allocate for that string. The Wasm program returns a memory address for the external code to write to, and gets a callback when it is done.

The proposal does actually mention UTF-8 encoded strings as one of the possible interface types. It also mentions ArrayBuffer and JSON. The JSON data is "parsed as if it were passed to `JSON.parse()`", which sort of implies UTF-16, I think. It remains to be seen how many Web APIs will actually provide the UTF-8 String argument type.

## Performance

I've done some benchmarking on decoding UTF-8 and UTF-16 bytes from an `ArrayBuffer` to a JavaScript string that can be written to a DOM API like `HTMLElement.prototype.innerText`. Details are [here](../demos/2020-04-string-encoding).

The summary is that UTF-16 is faster for non-ASCII text because it has to be re-encoded. But UTF-8 is faster for ASCII because it doesn't have to be re-encoded and being half the byte size makes it quicker to copy. (Note that non-English web pages still have to use ASCII text for DOM node tags and other Web API strings.)

The decoding time is quite quick overall, ranging from 0.07-0.44ms for 60kB of text.

## Backward compatibility

Using UTF-8 would require API changes to the String and Char libraries. For example `String.length` would return a different result for the same content.

This would break some existing apps, making it harder to port them to Wasm.

Porting existing apps to Wasm is also the best way to test and debug this Elm-in-WebAssembly project. That in itself is a big incentive to keep the same APIs for the core libraries! The loss of productivity would be enormous if we couldn't compile existing example apps and expect them to work.

Of course if we stick with JS-compatible strings for this reason, it could create problems like Python 2.7, which survived 12 years after the release of Python 3. In large part, that was because of breaking changes related to string encoding.

## Conclusions

I think for practical reasons I have to stick with JS-compatible strings for the development phase so that I can run existing apps.

Migrating to UTF-8 seems like a bigger change than migrating to WebAssembly!

This project may even need two versions of the String and Char libraries, perhaps selected by means of a compile option. I already have a C macro to switch between UTF-8 and UTF-16 implementations, although I haven't made much effort to keep UTF-8 updated.
