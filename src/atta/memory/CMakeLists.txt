cmake_minimum_required(VERSION 3.12)

set(ATTA_CORE_SOURCE
	memoryManager.cpp
	allocator.cpp
	allocators/stackAllocator.cpp
)

add_library(atta_memory_lib STATIC
	${ATTA_CORE_SOURCE}
)
atta_target_common(atta_memory_lib)
atta_add_libs(atta_memory_lib)

########## Testing ##########
atta_add_tests("tests/allocators.cpp")

