cmake_minimum_required(VERSION 3.12)

set(ATTA_CORE_SOURCE
	stringId.cpp

	math/bounds.cpp
	math/common.cpp
	math/matrix.cpp
	math/point.cpp
	math/quaternion.cpp
	math/ray.cpp
)

add_library(atta_core_lib STATIC
	${ATTA_CORE_SOURCE}
)
atta_target_common(atta_core_lib)
atta_add_libs(atta_core_lib)
target_link_libraries(atta_core_lib PUBLIC glfw)

########## Testing ##########
set(ATTA_CORE_TEST_SOURCES
	tests/stringId.cpp
	tests/math.cpp
)
# Add to global test
atta_add_tests(${ATTA_CORE_TEST_SOURCES})

# Create local test
atta_create_local_test(
	atta_core_test 
	"${ATTA_CORE_TEST_SOURCES}"
	"atta_core_lib"
)
