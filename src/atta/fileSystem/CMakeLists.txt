cmake_minimum_required(VERSION 3.12)

set(ATTA_EVENT_SYSTEM_SOURCE
	fileManager.cpp
)

add_library(atta_file_system_lib STATIC
	${ATTA_EVENT_SYSTEM_SOURCE}
)
atta_target_common(atta_file_system_lib)
atta_add_libs(atta_file_system_lib)
