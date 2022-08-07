cmake_minimum_required(VERSION 3.14)

set(ATTA_MEMORY_MODULE_SOURCES
    manager.cpp
    allocator.cpp
    allocatedObject.cpp
    allocators/stackAllocator.cpp
    allocators/poolAllocator.cpp
    allocators/poolAllocatorT.cpp
    allocators/mallocAllocator.cpp
    allocators/bitmapAllocator.cpp
)

add_library(atta_memory_module STATIC ${ATTA_MEMORY_MODULE_SOURCES})
atta_target_common(atta_memory_module)
atta_add_libs(atta_memory_module)

########## Testing ##########
set(ATTA_MEMORY_MODULE_TEST_SOURCES
    tests/stackAllocator.cpp
    tests/poolAllocator.cpp
    tests/bitmapAllocator.cpp
    tests/allocatedObject.cpp
    tests/speed.cpp
)
# Add to global test
atta_add_tests(${ATTA_MEMORY_MODULE_TEST_SOURCES})

# Create local test
atta_create_local_test(
    atta_memory_module_test
    "${ATTA_MEMORY_MODULE_TEST_SOURCES}"
    "atta_memory_module"
)
