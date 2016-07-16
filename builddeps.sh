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
if [[ `uname -m` == arm* ]];
then
  make -f GNUmakefile-cross
  make -f GNUmakefile-cross test
else
  make CXXFLAGS="-fPIC"
  make test
fi

# Done
cd "$ROOTDIR"
