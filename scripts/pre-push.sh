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


# Build and run tests for native and wasm, using as many parallel jobs as possible
make -j check
