#!/bin/bash

set -e
ROOTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# build program and library
cd "$ROOTDIR"
make

# build node module
cd "$ROOTDIR/node"
npm install
