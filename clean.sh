#!/bin/bash

set -e
ROOTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd "$ROOTDIR"
git clean -f -d -x

cd "$ROOTDIR/librsync"
git clean -f -d -x

cd "$ROOTDIR/cryptopp"
git clean -f -d -x
