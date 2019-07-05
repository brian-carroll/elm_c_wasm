#!/usr/bin/env bash
cd build
make ../dist/bin/test
../dist/bin/test "$@" | tee test.log
cd ..
