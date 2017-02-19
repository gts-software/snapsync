# snapsync

An excellent library and toolset for optimized transfers of single files and directory snapshots based on librsync.

Please have a look at the [snapsync](https://github.com/core-process/snapsync/tree/master/node) npm-module in case you want to use the library together with NodeJS. In that case, you do not have to build the library manually as we provide prebuilt binaries for a selected set of architectures.

## Setup

Please follow the instructions below to build the `snapsync` tools for further use.

```sh
# install a recent version of boost and its header files
sudo apt-get install libboost1.58-all-dev

# clone repository
git clone --recursive https://github.com/core-process/snapsync.git
cd snapsync

# build dependencies
./builddeps.sh

# build library and tools
make

# in case you want to build the NodeJS library too, simply run
./build.sh
```

The native tools `justsnap` and `justsync` are placed in the `build` directory.

## Usage

### Synchronize Files

*Scenario:* You have two devices (A and B) each storing a different version of a particular file, and you want to update the version on device B to the version provided in device A.

```sh
# Usage:
# 1) justsync signature <base:in> <signature:out> [block-length] [sum-length]
# 2) justsync delta <signature:in> <target:in> <patch:out>
# 3) justsync patch <patch:in> <base:in> <target:out>

# on device B: create signature file (and copy it to device A)
justsync signature bin-v1 bin-v1.sig 0 8

# on device A: create patch file (and copy it to device B)
justsync delta bin-v1.sig bin-v2 bin-v1v2.patch

# on device B: apply patch file to old version to generate new version
justsync patch bin-v1v2.patch bin-v1 bin-v2
```

*Note:* You can use this procedure with any filetype. In case you are planning to use archive files like tar or zip to synchronize directory structures, please use our optimized file format instead (see below).

### Packing and Unpacking Directory Structures

The archive format provided by the `justsnap` tool is optimized for the signature, delta and patch cycle provided by `justsync`. It works with recursive directory structures and stores filenames, access bitmasks and content of files. Additionally it sorts directories while creating the image file to avoid unnecessary deltas during the synchronization phase.

```sh
# Usage:
# 1) justsnap create <directory:in> <image:out>
# 2) justsnap extract <image:in> <directory:out>

# create image
justsnap create my-data my-data.img

# extract image
justsnap extract my-data.img my-data
```
