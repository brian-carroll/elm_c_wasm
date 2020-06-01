I've posted here a
[few](https://discourse.elm-lang.org/t/first-class-functions-in-webassembly/1577)
[times](https://discourse.elm-lang.org/t/elm-core-libs-in-webassembly/4443)
[before](https://discourse.elm-lang.org/t/basic-demo-of-elm-in-webassembly/4627)
about my project to compile Elm to WebAssembly.

The project consists of two GitHub repos, one for the [compiler](https://github.com/brian-carroll/elm-compiler) and one for the [core libraries](https://github.com/brian-carroll/elm_c_wasm).

None of this is official. I'm not part of the core team and, as far as I know, they have no plans to move to WebAssembly any time soon. This is a hobby project driven by my own curiosity.

### Summary of previous posts

- I am using C as an intermediate language. (At this point, everyone asks "why not Rust?"! Rust is great for preventing errors in _handwritten_ code but I found it an inappropriate target for automatically generated code. More details [here](https://github.com/brian-carroll/elm_c_wasm/#why-c-and-not-rust))

- In [this post](https://discourse.elm-lang.org/t/elm-core-libs-in-webassembly/4443), I described the custom Garbage Collector, and the parts of the core libraries I'd ported to Wasm.

- In [my last post](https://discourse.elm-lang.org/t/basic-demo-of-elm-in-webassembly/4627) I described the system architecture. The Elm runtime remains in JavaScript because WebAssembly doesn't have Web APIs yet. The Wasm app talks to the runtime through a JS wrapper.

- I also showed a demo of a very basic working app. It was "hand compiled" as I didn't have a working compiler yet.

### Latest news

My [latest demo](https://brian-carroll.github.io/elm_c_wasm/todo-mvc/) is actually fully compiled code!

It's a WebAssembly port of Evan's _TodoMVC_ example from a few years ago (here's his [original repo](https://github.com/evancz/elm-todomvc))


### Compiler changes

The forked compiler accepts `--output elm.c` as a command-line option as well as `--output elm.js` and `--output elm.html`. Once I have the C file, I use [Emscripten](https://emscripten.org/) to further compile it to WebAssembly. There are a few build steps that I coordinate using GNU `make`.

I ran into a few challenges with the current architecture, mainly to do with type information. The compiler has several different stages. I only worked on the last stage, code generation, to limit the scope. But this means I don't have access to type information because it has been dropped by then.

- Currently it's unsafe to use a Float parameter in an app-level `Msg` type. I have no way to tell Int from Float when passing messages from the JS runtime to the Wasm app.
- The `Time` module doesn't work because it uses Int for timestamps. Realistic values require at least 42 bits but I'm using 32 bits. Some low level details work out nicely that way, because Wasm pointers are 32 bits. And the `Json` and `Bitwise` libraries rrequire 32-bit integers as well.
- I need to be able to distinguish custom types from tuples and lists. I'm using runtime type detection, but I'd prefer not to.

More detail here: https://github.com/brian-carroll/elm-compiler#architecture-challenges


### Development Status

So is that it? Is it all working? Can I use it in production right now? Is it really fast? OMG!

Nope! Sorry!

- I'm still working through lots of implementation issues. For example I have not yet managed to get Richard Feldman's [elm-spa-example](https://github.com/rtfeldman/elm-spa-example) working. It's a great test-case because it's complex enough that if I have any bugs, it's bound to show them up!

- It's not fast yet. Before I can focus on that, I need to debug it and sort out some issues with the architecture (see "current focus" below).

### Current focus

A lot of the work I'm currently doing is on the JS/Wasm interface. Since I have the runtime in JS and the app in Wasm, the interface between the two is a major focus.

Two of the topics I'm thinking about:

Some of the objects passed from the JS runtime to the app are unserialisable. For example, DOM events are not serialisable because they contain cyclical references. It's all to do with how the Json library is implemented. I have something that works _most_ of the time! But I'm working on something more reliable.

Currently the app's `Model` is stored in JS but the `update` function is in Wasm. That means the model has to get passed from JS to Wasm and back again on every update cycle, getting serialised and deserialised along the way. The only reason it works this way is that it was quicker to get up and running, because I didn't need to change anything in the JS runtime.

### String encoding

The [original post](https://github.com/elm/projects#explore-webassembly) suggesting this project specifically mentions string encoding, and UTF-8 in particular. And there was some discussion of this in my [last post](https://discourse.elm-lang.org/t/elm-core-libs-in-webassembly/4443/7). I suggested that UTF-16 might have advantages, due to better compatibility with JS and most of the browser APIs.

I did some [benchmarking](https://github.com/brian-carroll/elm_c_wasm/tree/ebd9a466a8c30a140caa74cdd5f62b1afc6f7221/demos/2020-04-string-encoding) on both encodings, to get an idea of the performance implications.

There's not much performance difference. Based on the results, I initially wanted to go with UTF-8. But then I realised that every time I pick an app to test the compiler on, I would also have to migrate its Elm code to using a new String library as well. Otherwise things like URL parsing might break, and who knows what else? I already have _so many things_ to debug, just changing the compile target. Changing a core library API as well? It just makes things too complicated. So I'm sticking with UTF-16 for this project. UTF-8 is a separate project.

### Asynchronous initialisation

WebAssembly modules are normally compiled [asynchronously](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/WebAssembly/instantiateStreaming) once loaded into the browser. We have to wait until the compilation is finished before we can call `Elm.Main.init`.

I created a new function `Elm.onReady` to help with this. You just put your app's normal setup code in a callback, and `Elm.onReady` will execute it at the right time.

For my WebAssembly version of the TodoMVC example, it looks like this:

```html
<script type="text/javascript">
  Elm.onReady(function () {
    var storedState = localStorage.getItem('elm-todo-save');
    var startingState = storedState ? JSON.parse(storedState) : null;
    var app = Elm.Main.init({ flags: startingState });
    app.ports.setStorage.subscribe(function (state) {
      localStorage.setItem('elm-todo-save', JSON.stringify(state));
    });
  });
</script>
```

### Summary

- We can compile some Elm apps to WebAssembly, including the [TodoMVC demo](https://brian-carroll.github.io/elm_c_wasm/todo-mvc/)
- There are some architecture issues to work out, there's no performance work done yet, and there's lots of kernel code unwritten.
- Wasm _enables_ UTF-8 but it's a separate project
- There are some changes in the API due to async compilation
