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
private:
  std::shared_ptr<protoboard<FieldT>> _pb;
  pb_variable_array<FieldT> _hash_packed;
  pb_variable_array<FieldT> _preimage_bits;
  pb_variable<FieldT> _ZERO;
  std::shared_ptr<digest_variable<FieldT>> _result;
  std::shared_ptr<multipacking_gadget<FieldT>> _packer;
  std::shared_ptr<sha256_hash_gadget<FieldT>> _g;
  r1cs_ppzksnark_proving_key<default_r1cs_ppzksnark_pp> _pk;
  r1cs_ppzksnark_proof<default_r1cs_ppzksnark_pp> _proof;
public:
  Prover(string PK_PATH)
  {
    default_r1cs_ppzksnark_pp::init_public_params();
    import_data_from_file(_pk, PK_PATH);

    _pb.reset(new protoboard<FieldT>);
  };
  void generate_r1cs();
  void compute_proof(initializer_list<unsigned long>, initializer_list<unsigned long>);
  void output_proof();
};

void Prover::generate_r1cs() 
{
  _hash_packed.allocate(*_pb, 2, "hash packed");
  _preimage_bits.allocate(*_pb, PREIMAGESIZE, "preimage_bits");
  _pb->set_input_sizes(2+NAMESIZE);
  _ZERO.allocate(*_pb, "ZERO");
  _result.reset(new digest_variable<FieldT>(*_pb, 256, "result"));
  _packer.reset(new multipacking_gadget<FieldT>(*_pb, _result->bits, _hash_packed, 128, "packer"));
  _packer->generate_r1cs_constraints(true);
  _g.reset(new sha256_hash_gadget<FieldT>(*_pb, _ZERO, _preimage_bits, _result));
  _g->generate_r1cs_constraints();
}

void Prover::compute_proof(initializer_list<unsigned long> name_bit, initializer_list<unsigned long> pass_bit)
{
  const libff::bit_vector name_bv  = libff::int_list_to_bits(name_bit, 64);
  const libff::bit_vector pass_bv  = libff::int_list_to_bits(pass_bit, 64);

  for (size_t i = 0; i < NAMESIZE; i++) {
    _pb->val(_preimage_bits[i]) = name_bv[i];
    _pb->val(_preimage_bits[i+NAMESIZE]) = pass_bv[i];
  }
  _pb->val(_ZERO) = 0;
  _g->generate_r1cs_witness();
  _packer->generate_r1cs_witness_from_bits();

  _proof = r1cs_ppzksnark_prover<default_r1cs_ppzksnark_pp>(_pk, _pb->primary_input(), _pb->auxiliary_input());

}

void Prover::output_proof() 
{
  cout << "PackedHash:" << _hash_packed.get_vals(*_pb)[0] << ":" << _hash_packed.get_vals(*_pb)[1] << endl;
  stringstream ss("");
  ss << _proof;
  string base = base64_encode(ss.str());
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
  prover.generate_r1cs();
  prover.compute_proof(name_bit, pass_bit);
  prover.output_proof();
  return 0;
}
