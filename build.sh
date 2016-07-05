#!/bin/bash

set -e
ROOTDIR=`pwd`

# build dependencies
./builddeps.sh

# build program and library
make

# build node module
cd "$ROOTDIR/node"
npm install
npm build .

# build tests
cd "$ROOTDIR/tests/node"
npm install
npm run build-once

# done
cd "$ROOTDIR"
