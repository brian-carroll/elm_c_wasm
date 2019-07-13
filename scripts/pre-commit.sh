#!/usr/bin/env bash

function is_added() {
  ! git diff --quiet --cached $1
}

function is_modified() {
  ! git diff --quiet $1
}

if is_added README.md && ! is_modified README.md
then
  node scripts/update-readme-TOC.js
  git add README.md
fi
