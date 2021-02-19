@REM https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?view=msvc-150

if exist dist\exe (
  del /Q dist\exe\*
) else (
  mkdir dist\exe
)

CL /Z7 /Od /Fe"dist\exe\test" /Fodist\exe\ src\test\test.c src\kernel\core\core.c src\kernel\test\test.c src\kernel\json\json.c src\kernel\wrapper\wrapper.c /link /DEBUG:FULL
