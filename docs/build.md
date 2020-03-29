# Build process

This project is very raw and unfinished at the moment. It's not even close to being useful for real apps. Compiling an Elm app to Wasm involves a lot of awkward manual steps. The process is not at all streamlined or production ready and the resulting Wasm app may contain bugs. A lot of the core library functions are not supported and there is no clear list of what is supported and what isn't.

You have been warned!

That said, if you'd like to help out or just tinker with this project, here's how to get started.

- Install Emscripten

  - https://emscripten.org/docs/getting_started/downloads.html

* Clone and build the forked Elm compiler

  - Install Stack (https://www.haskellstack.org)
  - Clone the forked repo (https://github.com/brian-carroll/elm-compiler)
  - `cd` to the root directory
  - `stack init`
  - `stack build`

* Clone this repo

  - In a different directory to the compiler. Perhaps put the two directories beside each other if you like, it doesn't matter.

* Run `npm install`

  - We need a couple of NPM packages.
    - TypeScript for the wrapper (src/kernel/wrapper/wrapper.ts)
      - This file could eventually be replaced with its compiled JS version
    - `http-server` to serve assets including the .wasm file
      - If you prefer to use something else, go ahead

* Ensure you're using the forked Elm binary

  - The Makefile invokes `elm`
  - You can either change the Makefile or change your PATH, whichever you prefer

* Build the demo

  - `cd demos/wrapper`
  - `make`

* Run a development server and open the demo

  - `npx http-server` (still inside `demos/wrapper`)
  - Open a browser at http://localhost:8080
