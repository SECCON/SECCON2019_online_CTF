include_directories(.)


add_executable(
  keygen

  keygen.cpp
)
target_link_libraries(
  keygen

  snark
)
target_include_directories(
  keygen

  PUBLIC
  ${DEPENDS_DIR}/libsnark
  ${DEPENDS_DIR}/libsnark/depends/libfqfft
  ${DEPENDS_DIR}/libsnark/depends/libff
)

add_executable(
  fake_prover

  fake_prover.cpp
)
target_link_libraries(
  fake_prover

  snark
)
target_include_directories(
  fake_prover

  PUBLIC
  ${DEPENDS_DIR}/libsnark
  ${DEPENDS_DIR}/libsnark/depends/libfqfft
  ${DEPENDS_DIR}/libsnark/depends/libff
)

