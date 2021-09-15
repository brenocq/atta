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

add_library(atta_core STATIC
	${ATTA_CORE_SOURCE}
)
atta_target_common(atta_core)
atta_add_libs(atta_core)

########## Testing ##########
set(ATTA_CORE_TEST_SOURCES
	tests/stringId.cpp
	tests/math.cpp
)
# Add to global test
atta_add_tests(${ATTA_CORE_TEST_SOURCES})

# Create local tests
atta_create_local_test(
	atta_core_string_id_test 
	"tests/stringId.cpp"
	"atta_core"
)
atta_create_local_test(
	atta_core_math_test 
	"tests/math.cpp"
	"atta_core"
)
