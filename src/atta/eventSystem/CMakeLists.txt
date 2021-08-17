cmake_minimum_required(VERSION 3.12)

set(ATTA_EVENT_SYSTEM_SOURCE
	dispatcher.cpp
)

add_library(atta_event_system_lib STATIC
	${ATTA_EVENT_SYSTEM_SOURCE}
)
target_precompile_headers(atta_event_system_lib PRIVATE ${ATTA_PCH})

########## Testing ##########
add_executable(atta_event_system_test tests/test.cpp)
target_link_libraries(atta_event_system_test PRIVATE atta_event_system_lib gtest_main)
target_compile_definitions(atta_event_system_test PRIVATE ${ATTA_DEFINITIONS})
set_target_properties(
    atta_event_system_test 
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TESTS_OUTPUT_DIR}
)
gtest_discover_tests(atta_event_system_test)
