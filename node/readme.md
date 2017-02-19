# snapsync

An excellent library and toolset for optimized transfers of single files and directory snapshots based on librsync.

![npm downloads total](https://img.shields.io/npm/dt/snapsync.svg) ![npm version](https://img.shields.io/npm/v/snapsync.svg) ![npm license](https://img.shields.io/npm/l/snapsync.svg)

## Installation

Install the `snapsync` module via

```sh
npm install snapsync --save
```

or

```sh
yarn add snapsync
```

## Usage

### Synchronize Files

*Scenario:* You have two devices (A and B) each storing a different version of a particular file, and you want to update the version on device B to the version provided in device A.

```js
var snapsync = require('snapsync');

// API:
// 1) snapsync.sync.signature(<base:in>, <signature:out> [, block-length] [, sum-length])
// 2) snapsync.sync.delta(<signature:in>, <target:in>, <patch:out>)
// 3) snapsync.sync.patch(<patch:in>, <base:in>, <target:out>)

// on device B: create signature file (and copy it to device A)
snapsync.sync.signature('bin-v1', 'bin-v1.sig', 0, 8);

// on device A: create patch file (and copy it to device B)
snapsync.sync.delta('bin-v1.sig', 'bin-v2', 'bin-v1v2.patch');

// on device B: apply patch file to old version to generate new version
snapsync.sync.patch('bin-v1v2.patch', 'bin-v1', 'bin-v2');
```

*Note:* You can use this procedure with any filetype. In case you are planning to use archive files like tar or zip to synchronize directory structures, please use our optimized file format instead (see below).

### Packing and Unpacking Directory Structures

The archive format provided by the `justsnap` tool is optimized for the signature, delta and patch cycle provided by `justsync`. It works with recursive directory structures and stores filenames, access bitmasks and content of files. Additionally it sorts directories while creating the image file to avoid unnecessary deltas during the synchronization phase.

```js
var snapsync = require('snapsync');

// API:
// 1) snapsync.snap.create(<directory:in>, <image:out>)
// 2) snapsync.snap.extract(<image:in>, <directory:out>)

// create image
snapsync.snap.create('my-data', 'my-data.img');

// extract image
snapsync.snap.extract('my-data.img', 'my-data');
```
