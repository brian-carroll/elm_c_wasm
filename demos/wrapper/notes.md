task to be performed seems to have wrong callback in wrong place
Wasm version has GotTime where JS version has the anonymous closure in Task.map (`elm$core$Task$map$inner`)

Making wasm take author_project_Main_cmdTime directly from JS fixes it
(well except for the int 32 issue)

There are lots of places to possibly cut over to JS. Try them all and see what works, then debug.

This is broken in Wasm but works in JS

- `author$project$Main$cmdTime`

Try each of these in JS:

- `elm$core$Task$perform`
- `author$project$Main$GotTime`
- `elm$time$Time$now`

elm$core$Task$command
elm$core$Task$Perform
elm$core$Task$map
elm$core$Task$map$inner
elm$core$Task$andThen

Whichever ones don't work, go a level down
