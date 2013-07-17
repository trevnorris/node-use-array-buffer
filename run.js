var basics = require('./build/Release/basics');

var iter = 1e5;
var size = 1024 * 1024;
var t = process.hrtime();

for (var i = 0; i < iter; i++) {
  //new Buffer(size);
  basics.MakeArrayBuffer(size);
}

t = process.hrtime(t);

console.log((((t[0] * 1e9 + t[1]) / iter)|0) + ' ns/op');
console.log(t[0] + (t[1] / 1e9).toFixed(2) + ' total sec');
console.log('RSS: ' +
            (process.memoryUsage().rss / 1024 / 1024).toFixed(2) +
            ' MB');
