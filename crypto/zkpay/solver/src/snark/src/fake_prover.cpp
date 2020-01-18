#include <stdlib.h>
#include <iostream>

#include "libff/algebra/fields/field_utils.hpp"
#include "libsnark/zk_proof_systems/ppzksnark/r1cs_ppzksnark/r1cs_ppzksnark.hpp"
#include "libsnark/common/default_types/r1cs_ppzksnark_pp.hpp"
#include "libsnark/gadgetlib1/pb_variable.hpp"
#include <libsnark/gadgetlib1/gadgets/hashes/sha256/sha256_gadget.hpp>

#include "util.hpp"
#include "fake_util.hpp"
#include "sha256_hash_gadget.hpp"

using namespace libsnark;
using namespace std;

typedef libff::Fr<default_r1cs_ppzksnark_pp> FieldT;

int main(int argc, char *argv[])
{
  if (argc != (1+1+4)) {
    cout << "arg error" << endl;
    return -1;
  }

  const initializer_list<unsigned long> name_bit = {strtoul(argv[2], nullptr,0),
						    strtoul(argv[3], nullptr,0),
						    strtoul(argv[4], nullptr,0),
						    strtoul(argv[5], nullptr,0)};//256bit
  const initializer_list<unsigned long> pass_bit = {0, 0, 0, 0};//256bit

  default_r1cs_ppzksnark_pp::init_public_params();

  protoboard<FieldT> pb;

  pb_variable_array<FieldT> hash_packed;//packed hash value
  hash_packed.allocate(pb, 2, "hash packed");//allocate to pb

  const libff::bit_vector name_bv  = libff::int_list_to_bits(name_bit, 64);
  const libff::bit_vector pass_bv  = libff::int_list_to_bits(pass_bit, 64);

  pb_variable_array<FieldT> preimage_bits;//bit array to be hashed(512bit)
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
  
  // import proving key
  r1cs_ppzksnark_proving_key<default_r1cs_ppzksnark_pp> pk;
  import_data_from_file(pk, "./pk_data");

  auto ints = bit_list_to_ints(result->get_digest(), 32);
  r1cs_ppzksnark_proof<default_r1cs_ppzksnark_pp> proof;
  vector<FieldT> primary_input = {
    //arbitary created hash value
    FieldT("216895746203396674446478531858505072007"),FieldT("120531475434300841633452505446035459042"), 
    //name bit (256 bit)
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,0,1,1,0,0,1,0,0,0,1,1,0,1,1,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,1,1,0
  };
  proof = r1cs_ppzksnark_fake_prover<default_r1cs_ppzksnark_pp>(pk, primary_input, pb.auxiliary_input());

  stringstream ss("");
  ss << proof;
  string base = base64_encode(ss.str()); // why a proof contains line break?
  cout << "Proof:" << base;

  return 0;
}



