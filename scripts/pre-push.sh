#!/bin/bash
set -e

echo "Build & test binary and Wasm in parallel"

(make dist/bin/test ; echo -e "\nBinary tests" ; ./dist/bin/test --all) &
job_bin=$!

(make www ; echo -e "\nWasm tests" ; node ./src/test/test-runner.js --all) &
job_www=$!

wait $job_bin
wait $job_www
