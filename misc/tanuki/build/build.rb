require 'test/unit/assertions'
require 'zlib'
require 'priority_queue'

include Test::Unit::Assertions

def b(n, size)
  n.to_s(2).rjust(size, '0')
end

# https://rosettacode.org/wiki/Huffman_coding#Ruby
def do_huffman_encoding(char_count)
  padding_index = 0
  while char_count.size < 2
    unless char_count.has_key? padding_index
      char_count[padding_index] = 0
    end
    padding_index += 1
  end
  pq = PriorityQueue.new
  char_count.each {|char, count| pq.push(char, count)}
 
  while pq.length > 1
    key1, prio1 = pq.delete_min
    key2, prio2 = pq.delete_min
    pq.push([key1, key2], prio1 + prio2)
  end

  Hash[*generate_table(pq.min_key)]
end

def huffman_encoding(char_count, max_size)
  encoding_huffman = do_huffman_encoding(char_count)
  threshold = 2
  until encoding_huffman.all? {|k, v| v <= max_size} do
    encoding_huffman = do_huffman_encoding(char_count.transform_values! {|v| [v, threshold].max})
    threshold += 1
  end
  encoding_huffman
end
 
def generate_table(ary, depth = 0)
  case ary
  when Array
    generate_table(ary[0], depth + 1) + generate_table(ary[1], depth + 1)
  else
    [ary, depth]
  end
end

def generate_encoding(table)
  prev_value = 0
  prev_width = 0
  table.sort_by {|k, v| [v, k]} .each_with_index.map do |(k, v), i|
    if i != 0
      prev_value += 1
      prev_value <<= v - prev_width
    end
    prev_width = v
    [k, b(prev_value, v)]
  end .to_h
end

def encode_len_table(list)
  list.chunk(&:itself).to_a.map do |value, list|
    assert value <= 15
    count = list.size
    if value == 0
      if count <= 2
        [0] * count
      elsif count <= 10
        [17, b(count - 3, 3).reverse]
      elsif count <= 138
        [18, b(count - 11, 7).reverse]
      elsif count <= 140
        [18, b(138 - 11, 7).reverse, *([0] * (count - 138))]
      elsif count <= 148
        [18, b(138 - 11, 7).reverse, 17, b(count - 138 - 3, 3).reverse]
      elsif count <= 276
        [18, b(138 - 11, 7).reverse, 18, b(count - 138 - 11, 7).reverse]
      else
        assert false
      end
    else
      if count <= 3
        [value] * count
      else
        [
          value,
          *([value] * ((count - 1) % 6)),
          *([16, b(3, 2).reverse] * ((count - 1) / 6)),
        ]
      end
    end
  end .flatten
end

def get_huffman_block(lit_table, dist_table)
  lit_size = [*lit_table.keys, 256].max + 1
  dist_size = [*dist_table.keys, 0].max + 1
  lit_list = lit_size.times.map do |i|
    lit_table.fetch(i, 0)
  end
  dist_list = dist_size.times.map do |i|
    dist_table.fetch(i, 0)
  end
  lit_encoded = encode_len_table(lit_list)
  dist_encoded = encode_len_table(dist_list)
  tally = [*lit_encoded, *dist_encoded].select {|value| value.is_a? Integer} .group_by(&:itself).transform_values(&:count)

  encoding_huffman = huffman_encoding(tally, 7)
  assert encoding_huffman.all? {|k, v| v <= 7}, {huffman: encoding_huffman, tally: tally}.inspect
  encoding_table = generate_encoding(encoding_huffman)

  lit_data, dist_data = [lit_encoded, dist_encoded].map do |encoded|
    encoded.map do |value|
      if value.is_a? Integer
        encoding_table[value]
      else
        value
      end
    end .join
  end

  encoding_list = [16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15].map do |value|
    encoding_huffman.fetch(value, 0)
  end
  encoding_list = encoding_list[0..[encoding_list.rindex {|v| v != 0}, 3].max]

  [
    b(lit_size - 257, 5).reverse,
    b(dist_size - 1, 5).reverse,
    b(encoding_list.size - 4, 4).reverse,
    encoding_list.map {|v| b(v, 3).reverse},
    lit_data,
    dist_data,
  ].join
end

def lzss_compress(string)
  assert string.size <= 32768
  hash = {}
  compressed = []
  compressed_size = 0
  bytes = string.bytes

  bytes.each_cons(3).with_index do |con, i|
    hash[con] = i unless hash.has_key?(con)
    next if i < compressed_size

    if hash.has_key?(con) && hash[con] < i - 3
      # copy until overwrap
      copy_from = hash[con]
      len = 3
      while bytes[copy_from + len] == bytes[i + len] && len + 1 <= 258
        len += 1
      end
      compressed << {type: :copy, len: len, dist: i - copy_from}
      compressed_size += len
    else
      compressed << {type: :value, value: con[0]}
      compressed_size += 1
    end
  end

  while compressed_size < bytes.size
    compressed << {type: :value, value: bytes[compressed_size]}
    compressed_size += 1
  end

  compressed
end

class CRC32
  def initialize
    @crc = 0
    @size = 0
  end

  def normalize(data)
    ret = 0
    (data.bit_length - 1).downto(0) do |i|
      ret = (0xFFFFFFFF & (ret << 1)) ^ (0x04C11DB7 * ret[31]) ^ data[i]
    end
    ret
  end

  def normalize_bits(bits)
    ret = 0
    bits.chars do |c|
      ret = (0xFFFFFFFF & (ret << 1)) ^ (0x04C11DB7 * ret[31]) ^ c.to_i
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
    add(mul(a, pow(2, b_size)), b)
  end

  def concat_fast(a, b, exp_b_size)
    add(mul(a, exp_b_size), b)
  end

  def repeat(a, size, times)
    ret = 0
    ret_size = 0

    exp_size = pow(2, size) # 2 ** size
    exp_ret_size = 1 # 2 ** ret_size

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

  def to_crc32
    remainder = -@size % 8
    size = remainder == 0 ? @size : (@size / 8 * 8 + 8)
    ret = add(mul(concat(@crc, normalize_bits('0' * remainder), remainder), pow(2, 32)), mul(0xFFFFFFFF, pow(2, size)))
    (normalize(ret) ^ 0xFFFFFFFF).to_s(2).rjust(32, '0').reverse.to_i(2)
  end

  def crc32_repeat(data, times)
    to_crc32(repeat(normalize(data.unpack('b*')[0].to_i(2)), data.size * 8, times), data.size * times)
  end

  def push(bits)
    @crc = concat(@crc, normalize_bits(bits), bits.size)
    @size += bits.size
  end

  def push_repeat(bits, times)
    @crc = concat(@crc, repeat(normalize_bits(bits), bits.size, times), bits.size * times)
    @size += bits.size * times
  end
end

class BitStream
  attr_reader :tokens

  def initialize(header = '')
    @tokens = []
    @crc = CRC32.new
    @crc.push(header.unpack('b*')[0])
    @register = ''
  end

  def push(bits)
    @crc.push(bits)
    push_once(bits)
  end

  def push_once(bits)
    @register += bits
    if @register.size >= 8
      bytes = @register.size / 8
      data = [@register[0...(8 * bytes)]].pack('b*')
      if @tokens.size > 0 && @tokens.last[:times] == 1
        @tokens.last[:data] += data
      else
        @tokens << {
          data: data,
          times: 1,
        }
      end
      @register = @register[(8 * bytes)..-1].to_s
    end
  end

  def push_repeat(bits, times)
    if times == 0
      return
    end

    @crc.push_repeat(bits, times)

    lead = ''
    while @register.size != 0
      push_bits = [8 - @register.size, bits.size].min
      push_once(bits[0...push_bits])
      lead = bits[push_bits..-1]
      times -= 1

      if times == 0
        push_once(lead)
        return
      end
    end

    assert @register.empty?

    cycle_size = bits.size.lcm(8) / bits.size
    cycle_times = times / cycle_size
    cycle_remainder = times % cycle_size

    if cycle_times > 0
      cycle_data = (lead + bits * cycle_size)[0...(bits.size * cycle_size)]
      @tokens << { 
        data: [cycle_data].pack('b*'),
        times: cycle_times,
      }
    end

    push_once(lead + bits * cycle_remainder)
  end

  # align into byte boundary
  def align
    if @register.size % 8 != 0
      push('0' * (8 - @register.size % 8))
    end
  end

  def to_s
    repeated_tokens = @tokens.map do |token|
      token[:data] * token[:times]
    end
    unless @register.empty?
      repeated_tokens << [@register.ljust(8, '0')].pack('b*')
    end
    repeated_tokens.join
  end

  def crc
    @crc.to_crc32
  end
end

def get_fixed_huffman(n)
  if n <= 143
    b(0b00110000 + n, 8)
  elsif n <= 255
    b(0b110010000 + n - 144, 9)
  elsif n <= 279
    b(0b0000000 + n - 256, 7)
  else
    b(0b11000000 + n - 280, 8)
  end
end

def get_len_literal(n)
  assert(n >= 3, n.inspect)
  assert(n <= 258, n.inspect)
  if n == 258
    [285, '']
  elsif n < 11
    [257 + n - 3, '']
  elsif n < 19
    [265 + (n - 11) / 2, b((n - 11) % 2, 1).reverse]
  elsif n < 35
    [269 + (n - 19) / 4, b((n - 19) % 4, 2).reverse]
  elsif n < 67
    [273 + (n - 35) / 8, b((n - 35) % 8, 3).reverse]
  elsif n < 131
    [277 + (n - 67) / 16, b((n - 67) % 16, 4).reverse]
  else
    [281 + (n - 131) / 32, b((n - 131) % 32, 5).reverse]
  end
end

def len_fixed(n)
  assert(n >= 3, n.inspect)
  assert(n <= 258, n.inspect)
  literal, bits = get_len_literal(n)
  get_fixed_huffman(literal) + bits
end

def get_dist_literal(n)
  assert(n >= 1, n.inspect)
  assert(n <= 32768, n.inspect)
  if n < 5
    [n - 1, '']
  elsif n < 9
    [(n - 5) / 2 + 4, b((n - 5) % 2, 1).reverse]
  elsif n < 17
    [(n - 9) / 4 + 6, b((n - 9) % 4, 2).reverse]
  elsif n < 33
    [(n - 17) / 8 + 8, b((n - 17) % 8, 3).reverse]
  elsif n < 65
    [(n - 33) / 16 + 10, b((n - 33) % 16, 4).reverse]
  elsif n < 129
    [(n - 65) / 32 + 12, b((n - 65) % 32, 5).reverse]
  elsif n < 257
    [(n - 129) / 64 + 14, b((n - 129) % 64, 6).reverse]
  elsif n < 513
    [(n - 257) / 128 + 16, b((n - 257) % 128, 7).reverse]
  elsif n < 1025
    [(n - 513) / 256 + 18, b((n - 513) % 256, 8).reverse]
  elsif n < 2049
    [(n - 1025) / 512 + 20, b((n - 1025) % 512, 9).reverse]
  elsif n < 4097
    [(n - 2049) / 1024 + 22, b((n - 2049) % 1024, 10).reverse]
  elsif n < 8193
    [(n - 4097) / 2048 + 24, b((n - 4097) % 2048, 11).reverse]
  elsif n < 16385
    [(n - 8193) / 4096 + 26, b((n - 8193) % 4096, 12).reverse]
  else
    [(n - 16385) / 8192 + 28, b((n - 16385) % 8192, 13).reverse]
  end
end

def dist_fixed(n)
  assert(n >= 1, n.inspect)
  assert(n <= 32768, n.inspect)
  literal, bits = get_dist_literal(n)
  b(literal, 5) + bits
end

def get_len_bits(n)
  assert n <= 0xFFFF
  len = [n].pack('s').unpack('b*')[0]
  nlen = len.chars.map {|c| c == '0' ? '1' : '0'}.join
  len + nlen
end

def get_raw_block(data, final: false)
  header = final ? '10000000' : '00000000'
  header + get_len_bits(data.size) + data.unpack('b*')[0]
end

flag = 'SECCON{D3FLaT3_1s_s0_COMPLEX,_B4by!}'
TA = "\xe3\x81\x9f".force_encoding('ascii-8bit')
TARGET_SIZE = ((5000_0000_0000_0000.0 / (7581.0 / 4.0)) * (2 ** 40)).to_i

seperators = flag.size.times.map {rand(TARGET_SIZE / 3)} .sort
sizes = [0, *seperators, TARGET_SIZE / 3].each_cons(2).map {|a, b| b - a + 16}
tokens = [{
  data: TA,
  times: sizes[0],
}]
flag.chars.each_with_index do |char, i|
  tokens << {
    data: char,
    times: 1,
  }
  tokens << {
    data: TA,
    times: sizes[i + 1],
  }
end
puts "Raw data: #{tokens.inspect}"

initial_crc = CRC32.new
tokens.each do |token|
  initial_crc.push_repeat(token[:data].unpack('b*')[0], token[:times])
end
data_crc = initial_crc.to_crc32

stream = nil

COUNT = 15

COUNT.times do |count|
  data_size = tokens.map {|d| d[:data].size * d[:times]} .sum

  chunks = data_size < 25000 ? 1 : 10

  chunk_seperators = (chunks - 1).times.map {rand(data_size - chunks * 16)} .sort
  chunk_sizes = [0, *chunk_seperators, data_size - chunks * 16].each_cons(2).map {|a, b| b - a + 16}
  assert chunk_sizes.sum == data_size

  header = [
    31, # ID1
    139, # ID2
    8, # Compression Method = "deflate"
    '00010000', # flags = (FNAME)
    Time.now.to_i, # MTIME
    0, # Extra Flags
    3, # OS Type = "Unix"
    'tanuki.txt' + '.gz' * count, # Original File Name
  ].pack("cccb8VccZ*")

  stream = BitStream.new
  stream.push header.unpack('b*')[0]

  chunk_sizes.each_with_index do |size, i|
    current_size = 0
    chunk_tokens = []
    while current_size < size
      token = tokens.shift
      if token[:data].size * token[:times] <= size - current_size
        chunk_tokens << token
        current_size += token[:data].size * token[:times]
      else
        space = size - current_size
        times = space / token[:data].size
        if times != 0
          chunk_tokens << {
            data: token[:data],
            times: times,
          }
        end
        remainder = space % token[:data].size
        if remainder == 0
          tokens.unshift({
            data: token[:data],
            times: token[:times] - times,
          })
        else
          chunk_tokens << {
            data: token[:data][0...remainder],
            times: 1,
          }
          if token[:times] - times - 1 > 0
            tokens.unshift({
              data: token[:data],
              times: token[:times] - times - 1,
            })
          end
          tokens.unshift({
            data: token[:data][remainder..-1],
            times: 1,
          })
        end
        current_size = size
      end
    end

    if data_size < 25000
      type = 2
    elsif count == 0
      if i == 6
        type = 0
      elsif i == 3
        type = 1
      else
        type = 2
      end
    elsif count < 4
      type = 2
    elsif count == 5 && i == 5
      type = 1
    else
      type = rand(10) == 0 ? 0 : 2
    end
    is_final = i == chunk_sizes.size - 1

    if type == 0 # Straight-forward fixed-hufffman compression
      stream.push is_final ? '1' : '0'
      stream.push '10'

      chunk_tokens.each do |token|
        assert token[:data].size <= 32768
        data_huffman = lzss_compress(token[:data]).map do |datum|
          if datum[:type] === :value
            get_fixed_huffman(datum[:value])
          elsif datum[:type] === :copy
            len_fixed(datum[:len]) + dist_fixed(datum[:dist])
          end
        end.join

        if 258 % token[:data].size == 0 && token[:times] - 1 > 0
          per_chunk = 258 / token[:data].size
          remaining_tokens = (token[:times] - 1) % per_chunk

          stream.push data_huffman

          repetition_data = len_fixed(258) + dist_fixed(token[:data].size)
          stream.push_repeat repetition_data, (token[:times] - 1) / per_chunk

          if remaining_tokens < 3
            stream.push_repeat data_huffman, remaining_tokens
          else
            stream.push len_fixed(token[:data].size * remaining_tokens) + dist_fixed(token[:data].size)
          end
        else
          stream.push data_huffman
          if token[:times] > 1
            repetition_data = ''
            token[:data].bytes.each_slice(258).each do |slice|
              if slice.size < 3
                repetition_data += slice.map {|byte| get_fixed_huffman(byte)} .join
              else
                repetition_data += len_fixed(slice.size) + dist_fixed(token[:data].size)
              end
            end
            stream.push_repeat repetition_data, token[:times] - 1
          end
        end
      end

      stream.push get_fixed_huffman(0x100)
    elsif type == 1 # Raw
      stream.push (is_final && chunk_tokens.size == 1 && chunk_tokens[0][:times] == 1) ? '1' : '0'
      stream.push '00'
      stream.align

      stream.push get_len_bits(chunk_tokens[0][:data].size)
      stream.push chunk_tokens[0][:data].unpack('b*')[0]
      if chunk_tokens[0][:times] > 1
        if is_final && chunk_tokens.size == 1
          stream.push_repeat get_raw_block(chunk_tokens[0][:data]), chunk_tokens[0][:times] - 2
          stream.push get_raw_block(chunk_tokens[0][:data], final: true)
        else
          stream.push_repeat get_raw_block(chunk_tokens[0][:data]), chunk_tokens[0][:times] - 1
        end
      end

      chunk_tokens[1..-1].each_with_index do |token, j|
        if is_final && j == chunk_tokens.size - 2
          stream.push_repeat get_raw_block(token[:data]), token[:times] - 1
          stream.push get_raw_block(token[:data], final: true)
        else
          stream.push_repeat get_raw_block(token[:data]), token[:times]
        end
      end
    else # Custom-Huffman with optimal efficiency
      lit_count = Hash.new(0)
      dist_count = Hash.new(0)

      chunk_tokens.each do |token|
        assert token[:data].size <= 32768
        data_lit_count = Hash.new(0)
        data_dist_count = Hash.new(0)

        lzss_data = lzss_compress(token[:data])
        lzss_data.each do |datum|
          if datum[:type] === :value
            data_lit_count[datum[:value]] += 1
          elsif datum[:type] === :copy
            len_literal, _ = get_len_literal(datum[:len])
            dist_literal, _ = get_dist_literal(datum[:dist])
            data_lit_count[len_literal] += 1
            data_dist_count[dist_literal] += 1
          end
        end

        if 258 % token[:data].size == 0 && token[:times] - 1 > 0
          per_chunk = 258 / token[:data].size
          remaining_tokens = (token[:times] - 1) % per_chunk
          data_lit_count.each {|k, v| lit_count[k] += v}
          data_dist_count.each {|k, v| dist_count[k] += v}

          len_literal, _ = get_len_literal(258)
          dist_literal, _ = get_dist_literal(token[:data].size)
          lit_count[len_literal] += (token[:times] - 1) / per_chunk
          dist_count[dist_literal] += (token[:times] - 1) / per_chunk

          if remaining_tokens < 3
            data_lit_count.each {|k, v| lit_count[k] += v * remaining_tokens}
            data_dist_count.each {|k, v| dist_count[k] += v * remaining_tokens}
          else
            len_literal, _ = get_len_literal(token[:data].size * remaining_tokens)
            dist_literal, _ = get_dist_literal(token[:data].size)
            lit_count[len_literal] += 1
            dist_count[dist_literal] += 1
          end
        else
          data_lit_count.each {|k, v| lit_count[k] += v}
          data_dist_count.each {|k, v| dist_count[k] += v}
          if token[:times] > 1
            token[:data].bytes.each_slice(258).each do |slice|
              if slice.size < 3
                slice.each {|byte| lit_count[byte] += token[:times] - 1}
              else
                len_literal, _ = get_len_literal(slice.size)
                dist_literal, _ = get_dist_literal(token[:data].size)
                lit_count[len_literal] += token[:times] - 1
                dist_count[dist_literal] += token[:times] - 1
              end
            end
          end
        end
      end

      lit_count[0x100] += 1

      lit_huffman = huffman_encoding(lit_count, 15)
      dist_huffman = huffman_encoding(dist_count, 15)
      lit_table = generate_encoding(lit_huffman)
      dist_table = generate_encoding(dist_huffman)

      stream.push is_final ? '1' : '0'
      stream.push '01'
      stream.push get_huffman_block(lit_huffman, dist_huffman)

      chunk_tokens.each do |token|
        assert token[:data].size <= 32768
        data_huffman = lzss_compress(token[:data]).map do |datum|
          if datum[:type] === :value
            lit_table[datum[:value]]
          elsif datum[:type] === :copy
            len_literal, len_bits = get_len_literal(datum[:len])
            dist_literal, dist_bits = get_dist_literal(datum[:dist])
            lit_table[len_literal] + len_bits + dist_table[dist_literal] + dist_bits
          end
        end.join

        if 258 % token[:data].size == 0 && token[:times] - 1 > 0
          per_chunk = 258 / token[:data].size
          remaining_tokens = (token[:times] - 1) % per_chunk

          stream.push data_huffman

          len_literal, len_bits = get_len_literal(258)
          dist_literal, dist_bits = get_dist_literal(token[:data].size)
          repetition_data = lit_table[len_literal] + len_bits + dist_table[dist_literal] + dist_bits

          stream.push_repeat repetition_data, (token[:times] - 1) / per_chunk

          if remaining_tokens < 3
            stream.push_repeat data_huffman, remaining_tokens
          else
            len_literal, len_bits = get_len_literal(token[:data].size * remaining_tokens)
            dist_literal, dist_bits = get_dist_literal(token[:data].size)
            stream.push lit_table[len_literal] + len_bits + dist_table[dist_literal] + dist_bits
          end
        else
          stream.push data_huffman
          if token[:times] > 1
            repetition_data = ''
            token[:data].bytes.each_slice(258).each do |slice|
              if slice.size < 3
                repetition_data += slice.map {|byte| lit_table[byte]} .join
              else
                len_literal, len_bits = get_len_literal(slice.size)
                dist_literal, dist_bits = get_dist_literal(token[:data].size)
                repetition_data += lit_table[len_literal] + len_bits
                repetition_data += dist_table[dist_literal] + dist_bits
              end
            end
            stream.push_repeat repetition_data, token[:times] - 1
          end
        end
      end

      stream.push lit_table[0x100]
    end
  end

  stream.align

  stream.push [data_crc].pack('V').unpack('b*')[0]
  stream.push [data_size & 0xFFFFFFFF].pack('V').unpack('b*')[0]

  compressed_size = stream.tokens.map {|d| d[:data].size * d[:times]} .sum
  puts "Compression ##{count + 1}: #{data_size}B => #{compressed_size}B (#{(compressed_size.fdiv(data_size) * 100).round(2)}%)"
  # assert stream.crc == Zlib::crc32(stream.to_s)

  tokens = stream.tokens
  data_crc = stream.crc
end

gzip = stream.to_s

File.open('tanuki.txt' + '.gz' * COUNT, 'wb') do |f|
  f.write gzip
end

Zlib::gunzip(gzip)