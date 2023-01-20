#!/usr/bin/env bash
set -e
cd `pwd`/build
rm -rf *
cmake .. && make