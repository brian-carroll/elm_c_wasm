@REM https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?view=msvc-150

if not exist dist\exe (
  mkdir dist\exe
)

set build_dir="build\exe"

if not exist %build_dir% (
  mkdir %build_dir%
)

CL /Z7 /Od /Fe"dist\exe\test" /VERBOSE /DDEBUG /Fo%build_dir%\ src\test\test.c src\kernel\core\core.c src\kernel\elm-test\elm-test.c src\kernel\json\json.c src\kernel\wrapper\wrapper.c /link /DEBUG:FULL
