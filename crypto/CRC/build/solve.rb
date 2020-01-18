require 'test/unit/assertions'
require 'zlib'
require 'openssl'

include Test::Unit::Assertions

def crc32(crc, data)
  crc = 0xFFFFFFFF ^ crc
  data.bytes do |c|
    crc = crc ^ c
    8.times do
      crc = (crc >> 1) ^ (0xEDB88320 * (crc & 1))
    end
  end
  0xFFFFFFFF ^ crc
end

def normalize(data)
  ret = 0
  (data.bit_length - 1).downto(0) do |i|
    ret = (0xFFFFFFFF & (ret << 1)) ^ (0x04C11DB7 * ret[31]) ^ data[i]
  end
  ret
end

def xormul(a, b)
  if b == 0
    return 0
  end
  (b % 2 * a) ^ mul(2 * a, b / 2)
end

def mul(a, b)
  normalize(xormul(a, b))
end

def add(a, b)
  a ^ b
end

def pow(a, b)
  ret = 1
  while b > 0
    if b % 2 == 1
      ret = mul(ret, a)
    end
    a = mul(a, a)
    b = b / 2
  end
  ret
end

def concat(a, b, b_size)
  add(mul(a, pow(2, b_size * 8)), b)
end

def concat_fast(a, b, exp_b_size)
  add(mul(a, exp_b_size), b)
end

def repeat(a, size, times)
  ret = 0
  ret_size = 0

  exp_size = pow(2, size * 8) # 2 ** (size * 8)
  exp_ret_size = 1 # 2 ** (ret_size * 8)

  while times > 0
    if times % 2 == 1
      ret = concat_fast(a, ret, exp_ret_size)
      ret_size += size
      exp_ret_size = mul(exp_ret_size, exp_size)
    end
    a = concat_fast(a, a, exp_size)
    size *= 2
    exp_size = mul(exp_size, exp_size)
    times = times / 2
  end
  ret
end

def to_crc32(a, size)
  ret = add(mul(a, pow(2, 32)), mul(0xFFFFFFFF, pow(2, size * 8)))
  (normalize(ret) ^ 0xFFFFFFFF).to_s(2).rjust(32, '0').reverse.to_i(2)
end

def crc32_repeat(data, times)
  to_crc32(repeat(normalize(data.unpack('b*')[0].to_i(2)), data.size, times), data.size * times)
end

assert crc32(0, 'hogefugapiyo') == Zlib::crc32('hogefugapiyo')
assert crc32(0, 'TSG') == Zlib::crc32('TSG')
assert crc32(0, "\xff" * 334) == Zlib::crc32("\xff" * 334)
assert Zlib::crc32('hogefugapiyo') == crc32_repeat('hogefugapiyo', 1)
assert Zlib::crc32('hogefugapiyo' * 10) == crc32_repeat('hogefugapiyo', 10)
assert Zlib::crc32('hogefugapiyo' * 100) == crc32_repeat('hogefugapiyo', 100)
assert Zlib::crc32('hogefugapiyo' * 1000) == crc32_repeat('hogefugapiyo', 1000)
assert Zlib::crc32("\xff" * 334) == crc32_repeat("\xff", 334)
assert Zlib::crc32('I like Rubik\'s Cube' * 1000) == crc32_repeat('I like Rubik\'s Cube', 1000)
assert Zlib::crc32('I also like "yuri" animes' * 1000) == crc32_repeat('I also like "yuri" animes', 1000)
assert Zlib::crc32('I guess you\'ve already watched "Kannazuki no Miko"' * 1000) == crc32_repeat('I guess you\'ve already watched "Kannazuki no Miko"', 1000)
assert Zlib::crc32('which is the best yuri anime ever' * 1000) == crc32_repeat('which is the best yuri anime ever', 1000)
assert Zlib::crc32('If not, watch it RIGHT NOW!' * 1000) == crc32_repeat('If not, watch it RIGHT NOW!', 1000)

key = ''
%w(TSG is here at SECCON CTF!).each do |pass|
  crc = crc32_repeat(pass, ('1' * 10000).to_i)
  key += [crc.to_s(16).rjust(8, '0')].pack('H*')
  puts "#{pass}: 0x#{crc.to_s(16).rjust(8, '0')}"
end

aes = OpenSSL::Cipher::AES.new(192, :ECB)
aes.encrypt
aes.padding = 0
aes.key = key
encoded = (aes.update('SECCON{Ur_Th3_L0rd_0f_the_R1NGs}') + aes.final).unpack('H*')
puts "Encoded: #{encoded[0]}"

assert encoded[0] == '79833173d435b6c5d8aa08f790d6b0dc8c4ef525823d4ebdb0b4a8f2090ac81e'

aes = OpenSSL::Cipher::AES.new(192, :ECB)
aes.decrypt
aes.padding = 0
aes.key = key

decoded = aes.update(encoded.pack('H*')) + aes.final
puts "Decoded: #{decoded}"