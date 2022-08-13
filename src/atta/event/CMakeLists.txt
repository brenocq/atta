cmake_minimum_required(VERSION 3.14)

set(ATTA_EVENT_MODULE_SOURCE
    event.cpp
    interface.cpp
    manager.cpp
)

add_library(atta_event_module STATIC ${ATTA_EVENT_MODULE_SOURCE})
atta_target_common(atta_event_module)
atta_add_libs(atta_event_module)

########## Testing ##########
set(ATTA_EVENT_MODULE_TEST_SOURCES
    tests/test.cpp
)
# Add to global test
atta_add_tests(${ATTA_EVENT_MODULE_TEST_SOURCES})

# Create local test
atta_create_local_test(
    atta_event_module_test
    "${ATTA_EVENT_MODULE_TEST_SOURCES}"
    "atta_event_module"
)
