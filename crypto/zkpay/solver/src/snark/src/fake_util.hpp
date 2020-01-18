#include "libsnark/zk_proof_systems/ppzksnark/r1cs_ppzksnark/r1cs_ppzksnark.hpp"
#include "libff/algebra/curves/public_params.hpp"
#include <libfqfft/polynomial_arithmetic/naive_evaluate.tcc>

#include "libsnark/gadgetlib1/pb_variable.hpp"


#pragma once

using namespace libsnark;
using namespace libff;
using namespace std;

#define TAU_PATH "./tau_data"


template<typename FieldT>
qap_witness<FieldT> r1cs_to_qap_fake_witness_map(const r1cs_constraint_system<FieldT> &cs,
                                            const r1cs_primary_input<FieldT> &primary_input,
                                            const r1cs_auxiliary_input<FieldT> &auxiliary_input,
                                            const FieldT &d1,
                                            const FieldT &d2,
                                            const FieldT &d3)
{
    libff::enter_block("Call to r1cs_to_qap_fake_witness_map");

    /* sanity check */
    assert(cs.is_satisfied(primary_input, auxiliary_input));

    const std::shared_ptr<libfqfft::evaluation_domain<FieldT> > domain = libfqfft::get_evaluation_domain<FieldT>(cs.num_constraints() + cs.num_inputs() + 1);

    r1cs_variable_assignment<FieldT> full_variable_assignment = primary_input;
    full_variable_assignment.insert(full_variable_assignment.end(), auxiliary_input.begin(), auxiliary_input.end());

    libff::enter_block("Compute evaluation of polynomials A, B on set S");
    std::vector<FieldT> aA(domain->m, FieldT::zero()), aB(domain->m, FieldT::zero());

    /* account for the additional constraints input_i * 0 = 0 */
    for (size_t i = 0; i <= cs.num_inputs(); ++i)
    {
        aA[i+cs.num_constraints()] = (i > 0 ? full_variable_assignment[i-1] : FieldT::one());
    }
    /* account for all other constraints */
    for (size_t i = 0; i < cs.num_constraints(); ++i)
    {
        aA[i] += cs.constraints[i].a.evaluate(full_variable_assignment);
        aB[i] += cs.constraints[i].b.evaluate(full_variable_assignment);
    }
    libff::leave_block("Compute evaluation of polynomials A, B on set S");

    libff::enter_block("Compute coefficients of polynomial A");
    domain->iFFT(aA);
    libff::leave_block("Compute coefficients of polynomial A");

    libff::enter_block("Compute coefficients of polynomial B");
    domain->iFFT(aB);
    libff::leave_block("Compute coefficients of polynomial B");

    libff::enter_block("Compute ZK-patch");
    std::vector<FieldT> coefficients_for_H(domain->m+1, FieldT::zero());
#ifdef MULTICORE
#pragma omp parallel for
#endif

    FieldT t;
    {
      //std::ifstream tau_reader("tau");
      std::ifstream tau_reader(TAU_PATH);
      tau_reader >> t;
    }

    libff::print_indent(); std::cout << "*** LOADED TOXIC WASTE (tau) ***: " << t << std::endl;

    //t = libff::get_root_of_unity<FieldT>(domain->m);
    //t = domain->get_domain_element(1);
    libff::print_indent(); std::cout << "*** LOADED TOXIC WASTE (tau) ***: " << t << std::endl;

    FieldT A_at_t = libfqfft::evaluate_polynomial<FieldT>(domain->m, aA, t);
    /*
    FieldT A_at_t = 0;
    for (size_t i = 0; i < aA.size(); i++) {
      //A_at_t += (FieldT(aA.size())*aA[i])*t^i;
      A_at_t += aA[i]*t^i;
      std::cout << "aA[" << i << "]: " << aA[i] << std::endl;;
    }
    */
    std::cout << "A_at_t: " << (A_at_t) << std::endl;

    FieldT B_at_t = libfqfft::evaluate_polynomial<FieldT>(domain->m, aB, t);
    /*
    FieldT B_at_t = 0;
    for (size_t i = 0; i < aB.size(); i++) {
      //B_at_t += (FieldT(aB.size())*aB[i])*t^i;
      B_at_t += aB[i]*t^i;
      std::cout << "aB[" << i << "]: " << aB[i] << std::endl;;
    }
    */
    std::cout << "B_at_t: " << B_at_t << std::endl;



    
    /* add coefficients of the polynomial (d2*A + d1*B - d3) + d1*d2*Z */
    for (size_t i = 0; i < domain->m; ++i)
    {
        //coefficients_for_H[i] = d2*aA[i] + d1*aB[i];
    }
    //coefficients_for_H[0] -= d3;
    //domain->add_poly_Z(d1*d2, coefficients_for_H);
    libff::leave_block("Compute ZK-patch");

    libff::enter_block("Compute evaluation of polynomial A on set T");
    domain->cosetFFT(aA, FieldT::multiplicative_generator);
    libff::leave_block("Compute evaluation of polynomial A on set T");

    libff::enter_block("Compute evaluation of polynomial B on set T");
    domain->cosetFFT(aB, FieldT::multiplicative_generator);
    libff::leave_block("Compute evaluation of polynomial B on set T");

    libff::enter_block("Compute evaluation of polynomial H on set T");
    std::vector<FieldT> &H_tmp = aA; // can overwrite aA because it is not used later
#ifdef MULTICORE
#pragma omp parallel for
#endif
    for (size_t i = 0; i < domain->m; ++i)
    {
        //H_tmp[i] = aA[i]*aB[i];
    }
    std::vector<FieldT>().swap(aB); // destroy aB

    libff::enter_block("Compute evaluation of polynomial C on set S");
    std::vector<FieldT> aC(domain->m, FieldT::zero());
    for (size_t i = 0; i < cs.num_constraints(); ++i)
    {
        aC[i] += cs.constraints[i].c.evaluate(full_variable_assignment);
    }
    libff::leave_block("Compute evaluation of polynomial C on set S");

    libff::enter_block("Compute coefficients of polynomial C");
    domain->iFFT(aC);
    libff::leave_block("Compute coefficients of polynomial C");

    FieldT C_at_t = libfqfft::evaluate_polynomial<FieldT>(domain->m, aC, t);
    /*
    FieldT C_at_t = 0;
    for (size_t i = 0; i < aC.size(); i++) {
      //std::cout << "t^i: " << (t^i) << std::endl;;
      std::cout << "aC[" << i << "]: " << aC[i] << std::endl;;
      //C_at_t += (FieldT(aC.size())*aC[i])*t^i;
      C_at_t += aC[i]*t^i;
    }
    */
    std::cout << "C_at_t: " << C_at_t << std::endl;

    FieldT Z_at_t = domain->compute_vanishing_polynomial(t);
    std::cout << "Z_at_t: " << Z_at_t << std::endl;

    FieldT final_H_at_t = (A_at_t*B_at_t - C_at_t)*Z_at_t.inverse();
    std::cout << "final_H_at_t: " << final_H_at_t << std::endl;

    std::cout << "AB-C at t: " << (A_at_t*B_at_t - C_at_t) << std::endl;


    libff::enter_block("Compute evaluation of polynomial C on set T");
    domain->cosetFFT(aC, FieldT::multiplicative_generator);
    libff::leave_block("Compute evaluation of polynomial C on set T");

#ifdef MULTICORE
#pragma omp parallel for
#endif
    for (size_t i = 0; i < domain->m; ++i)
    {
        //H_tmp[i] = (H_tmp[i]-aC[i]);
    }

    libff::enter_block("Divide by Z on set T");
    //domain->divide_by_Z_on_coset(H_tmp);
    libff::leave_block("Divide by Z on set T");

    libff::leave_block("Compute evaluation of polynomial H on set T");

    libff::enter_block("Compute coefficients of polynomial H");
    //domain->icosetFFT(H_tmp, FieldT::multiplicative_generator);
    libff::leave_block("Compute coefficients of polynomial H");

    libff::enter_block("Compute sum of H and ZK-patch");
#ifdef MULTICORE
#pragma omp parallel for
#endif
    for (size_t i = 0; i < domain->m; ++i)
    {
        //coefficients_for_H[i] += H_tmp[i];
    }
    libff::leave_block("Compute sum of H and ZK-patch");

    libff::leave_block("Call to r1cs_to_qap_fake_witness_map");

    coefficients_for_H[0] = final_H_at_t;

    return qap_witness<FieldT>(cs.num_variables(),
                               domain->m,
                               cs.num_inputs(),
                               d1,
                               d2,
                               d3,
                               full_variable_assignment,
                               std::move(coefficients_for_H));
}


template <typename ppT>
r1cs_ppzksnark_proof<ppT> r1cs_ppzksnark_fake_prover(const r1cs_ppzksnark_proving_key<ppT> &pk,
                                                const r1cs_ppzksnark_primary_input<ppT> &primary_input,
                                                const r1cs_ppzksnark_auxiliary_input<ppT> &auxiliary_input)
{
    libff::enter_block("Call to r1cs_ppzksnark_prover");

#ifdef DEBUG
    assert(pk.constraint_system.is_satisfied(primary_input, auxiliary_input));
#endif

    /*
    const libff::Fr<ppT> d1 = libff::Fr<ppT>::random_element(),
        d2 = libff::Fr<ppT>::random_element(),
        d3 = libff::Fr<ppT>::random_element();
        */
    const libff::Fr<ppT> d1 = 0,
        d2 = 0,
        d3 = 0;

    libff::Fr<ppT> t;
    {
      //std::ifstream tau_reader("tau");
      std::ifstream tau_reader(TAU_PATH);
      tau_reader >> t;
    }

    libff::print_indent(); std::cout << "*** LOADED TOXIC WASTE (tau) ***: " << t << std::endl;

    libff::enter_block("Compute the polynomial H");
    const qap_witness<libff::Fr<ppT> > qap_wit = r1cs_to_qap_fake_witness_map(pk.constraint_system, primary_input, auxiliary_input, d1, d2, d3);
    libff::leave_block("Compute the polynomial H");

#ifdef DEBUG
    const libff::Fr<ppT> t = libff::Fr<ppT>::random_element();
    qap_instance_evaluation<libff::Fr<ppT> > qap_inst = r1cs_to_qap_instance_map_with_evaluation(pk.constraint_system, t);
    assert(qap_inst.is_satisfied(qap_wit));
#endif

    knowledge_commitment<libff::G1<ppT>, libff::G1<ppT> > g_A = pk.A_query[0] + qap_wit.d1*pk.A_query[qap_wit.num_variables()+1];
    knowledge_commitment<libff::G2<ppT>, libff::G1<ppT> > g_B = pk.B_query[0] + qap_wit.d2*pk.B_query[qap_wit.num_variables()+1];
    knowledge_commitment<libff::G1<ppT>, libff::G1<ppT> > g_C = pk.C_query[0] + qap_wit.d3*pk.C_query[qap_wit.num_variables()+1];

    libff::G1<ppT> g_H = libff::G1<ppT>::zero();
    libff::G1<ppT> g_K = (pk.K_query[0] +
                   qap_wit.d1*pk.K_query[qap_wit.num_variables()+1] +
                   qap_wit.d2*pk.K_query[qap_wit.num_variables()+2] +
                   qap_wit.d3*pk.K_query[qap_wit.num_variables()+3]);

#ifdef DEBUG
    for (size_t i = 0; i < qap_wit.num_inputs() + 1; ++i)
    {
        assert(pk.A_query[i].g == libff::G1<ppT>::zero());
    }
    assert(pk.A_query.domain_size() == qap_wit.num_variables()+2);
    assert(pk.B_query.domain_size() == qap_wit.num_variables()+2);
    assert(pk.C_query.domain_size() == qap_wit.num_variables()+2);
    assert(pk.H_query.size() == qap_wit.degree()+1);
    assert(pk.K_query.size() == qap_wit.num_variables()+4);
#endif

#ifdef MULTICORE
    const size_t chunks = omp_get_max_threads(); // to override, set OMP_NUM_THREADS env var or call omp_set_num_threads()
#else
    const size_t chunks = 1;
#endif

    libff::enter_block("Compute the proof");

    libff::enter_block("Compute answer to A-query", false);
    g_A = g_A + kc_multi_exp_with_mixed_addition<libff::G1<ppT>,
                                                 libff::G1<ppT>,
                                                 libff::Fr<ppT>,
                                                 libff::multi_exp_method_bos_coster>(
        pk.A_query,
        1, 1+qap_wit.num_variables(),
        qap_wit.coefficients_for_ABCs.begin(), qap_wit.coefficients_for_ABCs.begin()+qap_wit.num_variables(),
        chunks);
    libff::leave_block("Compute answer to A-query", false);

    libff::enter_block("Compute answer to B-query", false);
    g_B = g_B + kc_multi_exp_with_mixed_addition<libff::G2<ppT>,
                                                 libff::G1<ppT>,
                                                 libff::Fr<ppT>,
                                                 libff::multi_exp_method_bos_coster>(
        pk.B_query,
        1, 1+qap_wit.num_variables(),
        qap_wit.coefficients_for_ABCs.begin(), qap_wit.coefficients_for_ABCs.begin()+qap_wit.num_variables(),
        chunks);
    libff::leave_block("Compute answer to B-query", false);

    libff::enter_block("Compute answer to C-query", false);
    g_C = g_C + kc_multi_exp_with_mixed_addition<libff::G1<ppT>,
                                                 libff::G1<ppT>,
                                                 libff::Fr<ppT>,
                                                 libff::multi_exp_method_bos_coster>(
        pk.C_query,
        1, 1+qap_wit.num_variables(),
        qap_wit.coefficients_for_ABCs.begin(), qap_wit.coefficients_for_ABCs.begin()+qap_wit.num_variables(),
        chunks);
    libff::leave_block("Compute answer to C-query", false);

    libff::enter_block("Compute answer to H-query", false);
    g_H = g_H + libff::multi_exp<libff::G1<ppT>,
                                 libff::Fr<ppT>,
                                 libff::multi_exp_method_BDLO12>(
        pk.H_query.begin(), pk.H_query.begin()+qap_wit.degree()+1,
        qap_wit.coefficients_for_H.begin(), qap_wit.coefficients_for_H.begin()+qap_wit.degree()+1,
        chunks);
    
    std::cout << "g_H: " << g_H << std::endl;
    std::cout << "coefficients_for_H[0]: " << qap_wit.coefficients_for_H[0] << std::endl;
    g_H = qap_wit.coefficients_for_H[0]*libff::G1<ppT>::one();
    std::cout << "g_H: " << g_H << std::endl;
    libff::leave_block("Compute answer to H-query", false);

    libff::enter_block("Compute answer to K-query", false);
    g_K = g_K + libff::multi_exp_with_mixed_addition<libff::G1<ppT>,
                                                     libff::Fr<ppT>,
                                                     libff::multi_exp_method_bos_coster>(
        pk.K_query.begin()+1, pk.K_query.begin()+1+qap_wit.num_variables(),
        qap_wit.coefficients_for_ABCs.begin(), qap_wit.coefficients_for_ABCs.begin()+qap_wit.num_variables(),
        chunks);
    libff::leave_block("Compute answer to K-query", false);

    libff::leave_block("Compute the proof");

    libff::leave_block("Call to r1cs_ppzksnark_prover");

    r1cs_ppzksnark_proof<ppT> proof = r1cs_ppzksnark_proof<ppT>(std::move(g_A), std::move(g_B), std::move(g_C), std::move(g_H), std::move(g_K));
    proof.print_size();

    return proof;
}

