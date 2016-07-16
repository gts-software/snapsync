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
else
  make CXXFLAGS="-std=c++11 -fPIC"
  make test
fi


# Done
cd "$ROOTDIR"
