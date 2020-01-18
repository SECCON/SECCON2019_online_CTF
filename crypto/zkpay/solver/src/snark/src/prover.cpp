#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "libff/algebra/fields/field_utils.hpp"
#include "libsnark/zk_proof_systems/ppzksnark/r1cs_ppzksnark/r1cs_ppzksnark.hpp"
#include "libsnark/common/default_types/r1cs_ppzksnark_pp.hpp"
#include "libsnark/gadgetlib1/pb_variable.hpp"
#include <libsnark/gadgetlib1/gadgets/hashes/sha256/sha256_gadget.hpp>

#include "util.hpp"
#include "sha256_hash_gadget.hpp"

using namespace libsnark;
using namespace std;

typedef libff::Fr<default_r1cs_ppzksnark_pp> FieldT;

class Prover{
  r1cs_ppzksnark_proving_key<default_r1cs_ppzksnark_pp> _pk;
public:
  Prover(string PK_PATH)
  {
    default_r1cs_ppzksnark_pp::init_public_params();
    import_data_from_file(_pk, PK_PATH);
  };
  void prove(initializer_list<unsigned long>, initializer_list<unsigned long>);
};

void Prover::prove(initializer_list<unsigned long> name_bit, initializer_list<unsigned long> pass_bit)
{
  protoboard<FieldT> pb;
  pb_variable_array<FieldT> hash_packed;
  hash_packed.allocate(pb, 2, "hash packed");

  const libff::bit_vector name_bv  = libff::int_list_to_bits(name_bit, 64);
  const libff::bit_vector pass_bv  = libff::int_list_to_bits(pass_bit, 64);

  pb_variable_array<FieldT> preimage_bits;
  preimage_bits.allocate(pb, PREIMAGESIZE, "preimage_bits");
  for (size_t i = 0; i < NAMESIZE; i++) {
    pb.val(preimage_bits[i]) = name_bv[i];
    pb.val(preimage_bits[i+NAMESIZE]) = pass_bv[i];
  }

  pb.set_input_sizes(2+NAMESIZE);

  pb_variable<FieldT> ZERO;
  ZERO.allocate(pb, "ZERO");
  pb.val(ZERO) = 0;

  std::shared_ptr<digest_variable<FieldT>> result;
  result.reset(new digest_variable<FieldT>(pb, 256, "result"));

  multipacking_gadget<FieldT> packer(pb, result->bits, hash_packed, 128, "packer");
  packer.generate_r1cs_constraints(true);

  sha256_hash_gadget<FieldT> g(pb, ZERO, preimage_bits, result);
  g.generate_r1cs_constraints();
  g.generate_r1cs_witness();

  packer.generate_r1cs_witness_from_bits();

  cout << "PackedHash:" << hash_packed.get_vals(pb)[0] << ":" << hash_packed.get_vals(pb)[1] << endl;
  r1cs_ppzksnark_proof<default_r1cs_ppzksnark_pp> proof;
  proof = r1cs_ppzksnark_prover<default_r1cs_ppzksnark_pp>(_pk, pb.primary_input(), pb.auxiliary_input());

  stringstream ss("");
  ss << proof;
  string base = base64_encode(ss.str()); // why a proof contains line break?
  cout << "Proof:" << base;
}


int main(int argc, char *argv[])
{
  if (argc != (1+1+4+4)) {
    cout << "arg error" << endl;
    return -1;
  }

  const initializer_list<unsigned long> name_bit = {strtoul(argv[2], nullptr, 0),
						    strtoul(argv[3], nullptr,0),
						    strtoul(argv[4], nullptr,0),
						    strtoul(argv[5], nullptr,0)};//256bit
  const initializer_list<unsigned long> pass_bit = {strtoul(argv[6], nullptr,0),
						    strtoul(argv[7], nullptr,0),
						    strtoul(argv[8], nullptr,0),
						    strtoul(argv[9], nullptr,0)};//256bit

  Prover prover(argv[1]);
  prover.prove(name_bit, pass_bit);
  return 0;
}

