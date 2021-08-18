cmake_minimum_required(VERSION 3.12)

set(ATTA_CORE_SOURCE
	log.cpp
	stringId.cpp
)

add_library(atta_core_lib STATIC
	${ATTA_CORE_SOURCE}
)
target_precompile_headers(atta_core_lib PUBLIC ${ATTA_PCH})
target_include_directories(atta_core_lib PUBLIC
	${ATTA_PATH}/src
)

########## Testing ##########
add_executable(atta_core_stringId_test tests/stringId.cpp)
target_link_libraries(atta_core_stringId_test PRIVATE atta_core_lib gtest_main)
target_compile_definitions(atta_core_stringId_test PRIVATE ${ATTA_DEFINITIONS})
set_target_properties(
    atta_core_stringId_test 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TESTS_OUTPUT_DIR}
)
gtest_discover_tests(atta_core_stringId_test)
