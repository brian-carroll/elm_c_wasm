@REM https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?view=msvc-150

if exist dist\exe (
  del /Q dist\exe\*
) else (
  mkdir dist\exe
)

set build_dir="build\exe"

if exist %build_dir% (
  del /Q %build_dir%\*
) else (
  mkdir %build_dir%
)

CL /Z7 /Od /Fe"dist\exe\test" /Fo%build_dir%\ src\test\test.c src\kernel\core\core.c src\kernel\test\test.c src\kernel\json\json.c src\kernel\wrapper\wrapper.c /link /DEBUG:FULL
