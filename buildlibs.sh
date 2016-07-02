#!/bin/bash

set -e
ROOTDIR=`pwd`

# Build librsync
cd "$ROOTDIR/librsync"
cmake .
make
make check

# Build cryptopp
cd "$ROOTDIR/cryptopp"
cmake .
make

# Done
cd "$ROOTDIR"
