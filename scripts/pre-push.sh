#!/bin/bash
set -e

(make dist/bin/test ; ./dist/bin/test --all) &
job_bin=$!

(make www ; node ./dist/www/test.js --all) &
job_www=$!

wait $job_bin
wait $job_www
