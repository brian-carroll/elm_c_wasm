#!/bin/bash
set -e

# Stash changes before running tests & unstash on exit

stash_stdout=$(git stash --include-untracked)
function cleanup() {
  if [ "$stash_stdout" != "No local changes to save" ]
  then
    git stash pop
  fi
}
trap cleanup EXIT


# Run tests for native and wasm concurrently

(make dist/bin/test ; ./dist/bin/test --all) &
job_bin=$!

(make wasm ; node ./dist/wasm/test.js --all) &
job_wasm=$!

wait $job_bin
wait $job_wasm
