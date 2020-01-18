#include <stdlib.h>
#include <iostream>

#include "libff/algebra/fields/field_utils.hpp"
#include "libsnark/zk_proof_systems/ppzksnark/r1cs_ppzksnark/r1cs_ppzksnark.hpp"
#include "libsnark/common/default_types/r1cs_ppzksnark_pp.hpp"
#include "libsnark/gadgetlib1/pb_variable.hpp"

#include "util.hpp"

using namespace libsnark;
using namespace std;

typedef libff::Fr<default_r1cs_ppzksnark_pp> FieldT;

template<typename FieldT>
void deserialize_bit_vector(libff::bit_vector in, vector<FieldT> &v);

class Verifier {
  r1cs_ppzksnark_verification_key<default_r1cs_ppzksnark_pp> _vk;
public:
  Verifier(string VK_PATH)
  {
    import_data_from_file(_vk, VK_PATH);
  }
  void verify(vector<FieldT>, initializer_list<unsigned long>, string);
};

void Verifier::verify(vector<FieldT> packed_hash_value, const initializer_list<unsigned long> name_input, string encodedProof)
{
  const libff::bit_vector name_bv  = libff::int_list_to_bits(name_input, 64);

  vector<FieldT> name_bit;
  deserialize_bit_vector(name_bv, name_bit);

  vector<FieldT> primary_input;
  primary_input.push_back(packed_hash_value[0]);
  primary_input.push_back(packed_hash_value[1]);
  primary_input.insert(primary_input.end(), name_bit.begin(), name_bit.end());

  string buf;
  string proof_str;
  r1cs_ppzksnark_proof<default_r1cs_ppzksnark_pp> proof;
  buf = base64_decode(encodedProof);

  stringstream ss("");
  ss << buf;
  ss >> proof;

  bool verified = r1cs_ppzksnark_verifier_strong_IC<default_r1cs_ppzksnark_pp>(_vk, primary_input, proof);

  if (verified) {
    cout << "Verification success." << endl;
  }
  else {
    cout << "Verification failed." << endl;
  }
}

int main(int argc, char *argv[])
{
  if (argc != 1+1+2+4+1) {
    cout << "arg error" << endl;
    return 0;
  }
  default_r1cs_ppzksnark_pp::init_public_params();

  vector<FieldT> packed_hash_value = {FieldT(argv[2]), FieldT(argv[3])};
  const initializer_list<unsigned long> name_input = {strtoul(argv[4], nullptr, 0),
						      strtoul(argv[5], nullptr,0),
						      strtoul(argv[6], nullptr,0),
						      strtoul(argv[7], nullptr,0)};
  string encodedProof = argv[8];
  Verifier verifier(argv[1]);
  verifier.verify(packed_hash_value, name_input, encodedProof);

  return 0;
}

template<typename FieldT>
void deserialize_bit_vector(libff::bit_vector in, vector<FieldT> &v)
{
    size_t size;
    size = in.size();
    v.resize(size);
    bool b;
    for (size_t i = 0; i < size; ++i)
    {
        if (in[i]) v[i] = 1;
        else v[i] = 0;
    }
}
