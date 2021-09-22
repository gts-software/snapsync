# gts-snapsync

An excellent library and toolset for optimized transfers of single files and directory snapshots based on librsync.

![npm downloads total](https://img.shields.io/npm/dt/gts-snapsync.svg) ![npm version](https://img.shields.io/npm/v/gts-snapsync.svg) ![npm license](https://img.shields.io/npm/l/gts-snapsync.svg)


## Installation

Install the `gts-snapsync` module via

```sh
npm install gts-snapsync --save
```

or

```sh
yarn add gts-snapsync
```

See the original documentation at https://github.com/core-process/snapsync  
This fork added async features to all the functions of snapsync.
Just add a callback function as the last parameter, for example:

Sync:
```js
snapsync.snap.create('dir', 'file');
```
Async:
```js
snapsync.snap.create('dir', 'file', (error) => {
	if (error) { console.log(error) }
});
// or 
await snapsync.snap.asyncCreate('dir','file');
```

## Changelog
> #### v1.2.3
> - Added support for Node 12+
> - Dropped support for Node < 8
> - Added `custom_modules` containing a modified version of `node-pre-gyp-github` since the original does not work anymore.
> - Use latest version of `@octokit/rest` to work with Github API