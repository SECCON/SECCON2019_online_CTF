const fs = require('fs');
const {inflate} = require('./lib/zlib.js'); // patched to emit tokens

let gzip = fs.readFileSync('../files/tanuki.txt.gz.gz.gz.gz.gz.gz.gz.gz.gz.gz.gz.gz');

for (const i of Array(12).keys()) {
  const headerEnd = gzip.findIndex((byte, index) => index > 10 && byte === 0);
  const zlib = Buffer.concat([
    Buffer.from('789C', 'hex'),
    gzip.slice(headerEnd + 1, -8),
    gzip.slice(-4),
  ]);

  const {tokens} = inflate(zlib);

  const newBytes = [];
  for (const token of tokens) {
    if (token.type === 'data') {
      newBytes.push(token.value);
    }
    if (token.type === 'repeat') {
      if (token.distance <= token.length) {
        // data copy overlaps and is repeating previous data. just ignore them
      } else {
        newBytes.push(...newBytes.slice(-token.distance, -token.distance + token.length));
      }
    }
  }

  gzip = Buffer.from(newBytes);

  if (i === 11) {
    console.log(gzip.toString().replace(/た/g, ''));
  }
}