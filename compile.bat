@REM https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?view=msvc-150

if not exist "dist\exe\" mkdir dist\exe
@REM CL /Zi /W4 /O2
CL /Fe"dist\exe\test" /Fodist\exe\ /Fddist\exe\ src\test\test.c src\kernel\core\core.c src\kernel\test\test.c src\kernel\json\json.c src\kernel\wrapper\wrapper.c
