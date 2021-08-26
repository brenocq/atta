cmake_minimum_required(VERSION 3.12)

set(ATTA_MEMORY_SOURCE
	memoryManager.cpp
	allocator.cpp
	allocatedObject.cpp
	allocators/stackAllocator.cpp
	allocators/poolAllocator.cpp
	allocators/mallocAllocator.cpp
)

add_library(atta_memory_lib STATIC
	${ATTA_MEMORY_SOURCE}
)
atta_target_common(atta_memory_lib)
atta_add_libs(atta_memory_lib)

########## Testing ##########
atta_add_tests(
	"tests/stackAllocator.cpp"
	"tests/poolAllocator.cpp"
	"tests/allocatedObject.cpp"
	"tests/speed.cpp"
)

