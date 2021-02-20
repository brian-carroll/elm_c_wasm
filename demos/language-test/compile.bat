@REM https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?view=msvc-150

if exist dist (
  del /Q dist\test*
) else (
  mkdir dist
)

set build_dir="build\exe"

if exist %build_dir% (
  del /Q %build_dir%\*
) else (
  mkdir %build_dir%
)

set kernel="..\..\src\kernel"

CL /Z7 /Od /Fe"dist\test" /Fo%build_dir%\ build\codemods\elm.c ..\..\src\test\test-imports.c %kernel%\core\core.c %kernel%\elm-test\elm-test.c %kernel%\json\json.c %kernel%\wrapper\wrapper.c /link /DEBUG:FULL
