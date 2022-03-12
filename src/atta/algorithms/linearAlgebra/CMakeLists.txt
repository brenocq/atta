cmake_minimum_required(VERSION 3.12)

set(ATTA_LINEAR_ALGEBRA_SOURCES
    LU/LU.cpp
    QR/QR.cpp
    SVD/SVD.cpp
)

add_library(atta_linalg STATIC
    ${ATTA_LINEAR_ALGEBRA_SOURCES}
)
atta_target_common(atta_linalg)
atta_add_libs(atta_linalg)

########## Testing ##########
set(ATTA_ALGS_LINALG_TEST_SOURCES
    LU/test.cpp
    QR/test.cpp
    SVD/test.cpp
)
# Add to global test
atta_add_tests(${ATTA_ALGS_LINALG_TEST_SOURCES})

# Create local tests
atta_create_local_test(
    atta_linalg_test 
    "${ATTA_ALGS_LINALG_TEST_SOURCES}"
    "atta_linalg"
)
