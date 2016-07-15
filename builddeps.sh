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
make CXXFLAGS="-std=c++11 -fPIC"
make test

# Done
cd "$ROOTDIR"
