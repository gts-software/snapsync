#!/bin/bash

# prepare
set -e
ROOTDIR=`pwd`

# get data directory
SCRIPT=`realpath $0`
SCRIPTDIR=`dirname $SCRIPT`
DATADIR=`realpath $SCRIPTDIR/../data`

cd $DATADIR
echo "data directory: `pwd`"

# deleting old files
echo "deleting old files..."
rm -f npp-6.9.1.img
rm -f npp-6.9.2.img
rm -f npp-6.9.1.sig
rm -f npp-6.9.1-2.patch
rm -f npp-6.9.2.img-reconstructed
rm -r -f npp-6.9.2-reconstructed

# creating base and target image
echo "creating base and target image..."
../../build/justsnap create npp-6.9.1 npp-6.9.1.img
../../build/justsnap create npp-6.9.2 npp-6.9.2.img

# syncing images
echo "syncing images..."
../../build/justsync signature npp-6.9.1.img npp-6.9.1.sig 0 8
../../build/justsync delta npp-6.9.1.sig npp-6.9.2.img npp-6.9.1-2.patch
../../build/justsync patch npp-6.9.1-2.patch npp-6.9.1.img npp-6.9.2.img-reconstructed

# extract target image
echo "extract target image..."
../../build/justsnap extract npp-6.9.2.img-reconstructed npp-6.9.2-reconstructed

# comparing target directory with reconstructed directory
echo "comparing target directory with reconstructed directory..."
diff -r -q npp-6.9.2 npp-6.9.2-reconstructed | diffstat

# done
cd "$ROOTDIR"
