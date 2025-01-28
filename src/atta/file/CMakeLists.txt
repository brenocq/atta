cmake_minimum_required(VERSION 3.14)

set(ATTA_SCRIPT_MODULE_SOURCE
    interface.cpp
    manager.cpp

    watchers/fileWatcher.cpp
    watchers/nullFileWatcher.cpp
    watchers/linuxFileWatcher.cpp

    project/project.cpp
    project/projectSerializer.cpp

    serializer/section.cpp
    serializer/serializer.cpp
)

add_library(atta_file_module STATIC
    ${ATTA_SCRIPT_MODULE_SOURCE}
)

target_link_libraries(atta_file_module PUBLIC atta_graphics_module atta_event_module)
atta_target_common(atta_file_module)
atta_add_libs(atta_file_module)

########## Testing ##########
set(ATTA_FILE_MODULE_TEST_SOURCES
    tests/serializer.cpp
)
# Add to global test
atta_add_tests(${ATTA_FILE_MODULE_TEST_SOURCES})

# Create local test
atta_create_local_test(
    atta_file_module_test
    "${ATTA_FILE_MODULE_TEST_SOURCES}"
    "atta_file_module"
)
