#!/bin/bash

set -e
ROOTDIR=`pwd`

# build program and library
make

# build node module
cd "$ROOTDIR/node"
npm install
node-gyp rebuild

# done
cd "$ROOTDIR"
