#!/bin/bash

set -e
ROOTDIR=`pwd`

# build program and library
make

# build node module
cd "$ROOTDIR/node"
npm install
npm run build

# done
cd "$ROOTDIR"
