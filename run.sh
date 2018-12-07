#! /bin/bash
make ./dist/bin/test
./dist/bin/test "$@"
