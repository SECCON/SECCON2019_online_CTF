#include <fstream>

#include "libsnark/zk_proof_systems/ppzksnark/r1cs_ppzksnark/r1cs_ppzksnark.hpp"
#include "libff/algebra/curves/public_params.hpp"
#include <libfqfft/polynomial_arithmetic/naive_evaluate.tcc>

#include "libsnark/gadgetlib1/pb_variable.hpp"

#pragma once

using namespace libsnark;
using namespace libff;
using namespace std;

#define PREIMAGESIZE 512
#define NAMESIZE 256
#define PASSSIZE 256

template<typename FieldT>
pb_variable_array<FieldT> from_bits(std::vector<bool> bits, pb_variable<FieldT>& ZERO) {
  pb_variable_array<FieldT> acc;
  for (size_t i = 0; i < bits.size(); i++) {
    bool bit = bits[i];
    acc.emplace_back(bit ? ONE : ZERO);
  }
  return acc;
}

vector<unsigned long> bit_list_to_ints(vector<bool> bit_list, const size_t wordsize) {
  vector<unsigned long> res;
  size_t iterations = bit_list.size()/wordsize;
  for (size_t i = 0; i < iterations; ++i) {
    unsigned long current = 0;
    for (size_t j = 0; j < wordsize; ++j) {
      if (bit_list.size() == (i*wordsize+j)) break;
      current += (bit_list[i*wordsize+j] * (1ul<<(wordsize-1-j)));
    }
    res.push_back(current);
  }
  return res;
}

template<typename dataT>
void output_data_to_file(dataT key, string pathToFile) 
{
  ofstream key_data;
  key_data.open(pathToFile);
  key_data << key << endl;
  key_data.close();
}

template<typename dataT>
void import_data_from_file(dataT& data, string pathToFile)
{
  ifstream ifs(pathToFile);
  if (ifs.fail())
    {
      cerr << "import data error" << endl;
    }
  ifs >> data;
  ifs.close();
}

string base64_encode(string str)
{
    const static char* table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string result;
    string::iterator it    = str.begin();
    string::iterator itEnd = str.end();
 
    union {
        unsigned char buf[4];
        unsigned long dw;
    } value_union;
 
    while (it != itEnd) {
        value_union.dw = 0;
        int count = 1;
        value_union.buf[2] = (unsigned char)*it++;

        if (it != itEnd) {
            value_union.buf[1] = (unsigned char)*it++;
            count++;
        }

        if (it != itEnd) {
            value_union.buf[0] = (unsigned char)*it++;
            count++;
        }
 
        switch(count) {
        case 3:
            result += table[0x3f & (value_union.dw >> 18)];
            result += table[0x3f & (value_union.dw >> 12)];
            result += table[0x3f & (value_union.dw >>  6)];
            result += table[0x3f & (value_union.dw >>  0)];
            break;
 
        case 2:
            result += table[0x3f & (value_union.dw >> 18)];
            result += table[0x3f & (value_union.dw >> 12)];
            result += table[0x3f & (value_union.dw >>  6)];
            result += "=";
            it = itEnd;
            break;
 
        case 1:
            result += table[0x3f & (value_union.dw >> 18)];
            result += table[0x3f & (value_union.dw >> 12)];
            result += "==";
            it = itEnd;
            break;
        }
    }

    return result;
}


string base64_decode(string str)
{

    const static unsigned char table[256] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xfe, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3e, 0xff, 0xff, 0xff, 0x3f, 
        0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 
        0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 
        0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    };
 
    string result;
    string::iterator it    = str.begin();
    string::iterator itEnd = str.end();
 
    while(it != itEnd) {
        unsigned long dw = 0;
        int bit_count = 0;
        int count = 4;
        while(count--) {
            dw <<= 6;
            if (it != itEnd) {
                unsigned char b_char = table[*it++];
                if (64 > b_char) {
                    dw |= b_char;
                    bit_count += 6;
                }
                else if (0xfe == b_char) {
                    count++;
                    dw >>= 6;
                }
            }
        }
 
        unsigned char* ba_tmp = (unsigned char*)&dw;
        unsigned char ba_write[3] = {ba_tmp[2], ba_tmp[1], ba_tmp[0]};
        int byte = bit_count / 8;
        if (0 < byte) {
            result.append((char*)ba_write, byte);
        }
    }

    return(result);
}
