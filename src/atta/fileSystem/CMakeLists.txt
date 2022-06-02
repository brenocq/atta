cmake_minimum_required(VERSION 3.12)

set(ATTA_SCRIPT_SYSTEM_SOURCE
    fileManager.cpp

    watchers/fileWatcher.cpp
    watchers/nullFileWatcher.cpp
    watchers/linuxFileWatcher.cpp

    project/project.cpp
    project/projectSerializer.cpp

    serializer/serializable.cpp
    serializer/section.cpp
)

add_library(atta_file_system STATIC
    ${ATTA_SCRIPT_SYSTEM_SOURCE}
)

target_link_libraries(atta_file_system PRIVATE atta_graphics_system)
atta_target_common(atta_file_system)
atta_add_libs(atta_file_system)

########## Testing ##########
set(ATTA_FILE_SYSTEM_TEST_SOURCES
    tests/section.cpp
)
# Add to global test
atta_add_tests(${ATTA_FILE_SYSTEM_TEST_SOURCES})

# Create local test
atta_create_local_test(
    atta_file_system_test
    "${ATTA_FILE_SYSTEM_TEST_SOURCES}"
    "atta_file_system"
)
