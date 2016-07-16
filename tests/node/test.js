var snapsync = require('../../node');
var fs = require("fs");
var path = require("path");

var rmdir = function(dir) {
    var list = fs.readdirSync(dir);
    for(var i = 0; i < list.length; i++) {
        var filename = path.join(dir, list[i]);
        var stat = fs.statSync(filename);

        if(filename == "." || filename == "..") {
            // pass these files
        } else if(stat.isDirectory()) {
            // rmdir recursively
            rmdir(filename);
        } else {
            // rm fiilename
            fs.unlinkSync(filename);
        }
    }
    fs.rmdirSync(dir);
};

var exists = function(path) {
  try {
    fs.statSync(path);
    return true;
  }
  catch(e) {
    if(e.code == 'ENOENT') {
      return false;
    }
    throw e;
  }
};

process.chdir(__dirname + '/../data');
console.log('data directory: ' + process.cwd());

console.log('deleting old files...');
if(exists('npp-6.9.1.img')) fs.unlinkSync('npp-6.9.1.img');
if(exists('npp-6.9.2.img')) fs.unlinkSync('npp-6.9.2.img');
if(exists('npp-6.9.1.sig')) fs.unlinkSync('npp-6.9.1.sig');
if(exists('npp-6.9.1-2.patch')) fs.unlinkSync('npp-6.9.1-2.patch');
if(exists('npp-6.9.2.img-reconstructed')) fs.unlinkSync('npp-6.9.2.img-reconstructed');
if(exists('npp-6.9.2-reconstructed')) rmdir('npp-6.9.2-reconstructed');

console.log('creating base and target image...');
snapsync.snap.create('npp-6.9.1', 'npp-6.9.1.img');
snapsync.snap.create('npp-6.9.2', 'npp-6.9.2.img');

console.log('syncing images...');
snapsync.sync.signature('npp-6.9.1.img', 'npp-6.9.1.sig', 0, 8);
snapsync.sync.delta('npp-6.9.1.sig', 'npp-6.9.2.img', 'npp-6.9.1-2.patch');
snapsync.sync.patch('npp-6.9.1-2.patch', 'npp-6.9.1.img', 'npp-6.9.2.img-reconstructed');

console.log('extract target image...');
snapsync.snap.extract('npp-6.9.2.img-reconstructed', 'npp-6.9.2-reconstructed');
