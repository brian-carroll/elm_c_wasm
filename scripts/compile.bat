@REM https://docs.microsoft.com/en-us/cpp/build/reference/compiler-options-listed-by-category?view=msvc-150

set build_dir="dist"

if exist %build_dir% (
  del /Q %build_dir%\*
) else (
  mkdir %build_dir%
)

CL /Z7 /Od /Fe"dist\vdom-alloc" /Fo%build_dir%\ vdom-alloc.c ../src/kernel/core/core.c ../src/kernel/json/json.c ../src/test/test-compiled.c ../src/test/test-imports.c /link /DEBUG:FULL
