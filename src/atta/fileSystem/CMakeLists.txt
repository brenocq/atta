cmake_minimum_required(VERSION 3.12)

set(ATTA_SCRIPT_SYSTEM_SOURCE
    fileManager.cpp

    watchers/fileWatcher.cpp
    watchers/nullFileWatcher.cpp
    watchers/linuxFileWatcher.cpp

    project/project.cpp
    project/projectSerializer.cpp

    serializer/serializable.cpp
    )

add_library(atta_file_system STATIC
    ${ATTA_SCRIPT_SYSTEM_SOURCE}
    )
target_link_libraries(atta_file_system PRIVATE atta_graphics_system)
atta_target_common(atta_file_system)
atta_add_libs(atta_file_system)
