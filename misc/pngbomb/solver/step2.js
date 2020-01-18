const rl = require('readline').createInterface({
  input: process.stdin,
  console: false,
});


let bufpos = 0;
const buffer = Buffer.alloc(1024);

rl.on('line', function (line) {
  const is_sym = line.match(/^sym (..)/);
  const is_copy = line.match(/^copy ({.*})/);
  if (is_sym) {
    buffer.writeUInt8(parseInt(is_sym[1], 16), bufpos++);
  } else if (is_copy) {
    const json = JSON.parse(is_copy[1]);
    for (let i = 0; i < json.length; i++) {
      buffer.writeUInt8(buffer.readUInt8(bufpos - json.distance), bufpos++);
    }
  }
});

process.stdin.on('end', function () {
  console.log("P4");
  console.log((bufpos * 8) + " 16");

  const pbm = Buffer.alloc(bufpos);

  // swap B/W because it's pbm
  for (let i = 0; i < bufpos; i++) {
    pbm.writeUInt8((~buffer.readUInt8(i)) & 0xff, i);
  }

  for (let i = 0; i < 16; i++) {
    process.stdout.write(pbm);
  }
});

// vim:set ts=2 sw=2 expandtab:
