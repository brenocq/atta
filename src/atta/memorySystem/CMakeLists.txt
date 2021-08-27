cmake_minimum_required(VERSION 3.12)

set(ATTA_MEMORY_SYSTEM_SOURCES
	memoryManager.cpp
	allocator.cpp
	allocatedObject.cpp
	allocators/stackAllocator.cpp
	allocators/poolAllocator.cpp
	allocators/mallocAllocator.cpp
	allocators/bitmapAllocator.cpp
)

add_library(atta_memory_system_lib STATIC
	${ATTA_MEMORY_SYSTEM_SOURCES}
)
atta_target_common(atta_memory_system_lib)
atta_add_libs(atta_memory_system_lib)

########## Testing ##########
set(ATTA_MEMORY_SYSTEM_TEST_SOURCES
	tests/stackAllocator.cpp
	tests/poolAllocator.cpp
	tests/bitmapAllocator.cpp
	tests/allocatedObject.cpp
	tests/speed.cpp
)
# Add to global test
atta_add_tests(${ATTA_MEMORY_SYSTEM_TEST_SOURCES})

# Create local test
atta_create_local_test(
	atta_memory_system_test
	"${ATTA_MEMORY_SYSTEM_TEST_SOURCES}"
	"atta_memory_system_lib"
)
