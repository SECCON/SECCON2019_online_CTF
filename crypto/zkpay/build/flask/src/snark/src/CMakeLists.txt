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
  prover

  prover.cpp
)
target_link_libraries(
  prover

  snark
)
target_include_directories(
  prover

  PUBLIC
  ${DEPENDS_DIR}/libsnark
  ${DEPENDS_DIR}/libsnark/depends/libfqfft
  ${DEPENDS_DIR}/libsnark/depends/libff
)

add_executable(
  verifier

  verifier.cpp
)
target_link_libraries(
  verifier

  snark
)
target_include_directories(
  verifier

  PUBLIC
  ${DEPENDS_DIR}/libsnark
  ${DEPENDS_DIR}/libsnark/depends/libfqfft
  ${DEPENDS_DIR}/libsnark/depends/libff
)

