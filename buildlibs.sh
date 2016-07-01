#!/bin/bash

set -e
ROOTDIR=`pwd`

# Build librsync
cd "$ROOTDIR/librsync"
cmake .
make
make check

# Build libmhash
cd "$ROOTDIR/libmhash"
./configure LDFLAGS="-static"
make

# Done
cd "$ROOTDIR"
