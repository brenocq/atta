cmake_minimum_required(VERSION 3.14)

set(ATTA_UTILS_SOURCE
    math/bounds.cpp
    math/common.cpp
    math/matrix.cpp
    math/quaternion.cpp
    math/ray.cpp

    config.cpp
    log.cpp
    profiler.cpp
    stringId.cpp
    stringUtils.cpp
)

add_library(atta_utils STATIC ${ATTA_UTILS_SOURCE})
atta_target_common(atta_utils)
atta_add_libs(atta_utils)

########## Testing ##########
set(ATTA_UTILS_TEST_SOURCES
    tests/math.cpp
    tests/stringId.cpp
    tests/stringUtils.cpp
)
# Add to global test
atta_add_tests(${ATTA_UTILS_TEST_SOURCES})

# Create local tests
atta_create_local_test(
    atta_utils_string_id_test
    "tests/stringId.cpp"
    "atta_utils"
)
atta_create_local_test(
    atta_utils_string_utils_test
    "tests/stringUtils.cpp"
    "atta_utils"
)
atta_create_local_test(
    atta_utils_math_test
    "tests/math.cpp"
    "atta_utils"
)
atta_create_local_test(
    atta_utils_quaternion_test
    "tests/quaternion.cpp"
    "atta_utils"
)
