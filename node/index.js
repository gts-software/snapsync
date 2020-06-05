var binary = require('node-pre-gyp');
var path = require('path');
var bindingPath = binary.find(path.resolve(path.join(__dirname,'./package.json')));
var binding = require(bindingPath);
var promisify = require('util').promisify;
// Promisify all functions
// Assumed the binding object has the following structure:
// {
//   snap: { create: [Function], extract: [Function] },
//   sync: { signature: [Function], delta: [Function], patch: [Function] }
// }
for (let m of Object.keys(binding)) {
    for (let f of Object.keys(binding[m])) {
        const asyncFunctionName = 'async' + f.replace(/^./, f[0].toUpperCase());
        binding[m][asyncFunctionName] = promisify(binding[m][f]);
    }
}
module.exports = binding;
