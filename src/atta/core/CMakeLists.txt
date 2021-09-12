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
target_link_libraries(atta_core PUBLIC glfw)

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

########## Public headers ##########
set(ATTA_CORE_PUBLIC_HEADERS
	math/bounds.h
	math/common.h
	math/math.h
	math/matrix.h
	math/matrix.inl
	math/point.h
	math/quaternion.h
	math/ray.h
	math/vector.h
	math/vector.inl
	math/vector2.inl
	math/vector3.inl
	math/vector4.inl
	assert.h
	common.h
	log.h
	log.inl
	stringId.h
	stringId.inl
)
atta_install(FILES ${ATTA_CORE_PUBLIC_HEADERS} DESTINATION core)
