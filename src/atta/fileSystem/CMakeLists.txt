cmake_minimum_required(VERSION 3.12)

set(ATTA_EVENT_SYSTEM_SOURCE
	fileManager.cpp
	watcher/fileWatcher.cpp
	watcher/nullFileWatcher.cpp
	watcher/linuxFileWatcher.cpp
)

add_library(atta_file_system STATIC
	${ATTA_EVENT_SYSTEM_SOURCE}
)
atta_target_common(atta_file_system)
atta_add_libs(atta_file_system)
