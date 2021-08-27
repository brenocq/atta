cmake_minimum_required(VERSION 3.12)

set(ATTA_SERVICE_LOCATOR_SOURCE
	serviceLocator.cpp
	service.cpp
)

add_library(atta_service_locator_lib STATIC
	${ATTA_SERVICE_LOCATOR_SOURCE}
)
atta_target_common(atta_service_locator_lib)
atta_add_libs(atta_service_locator_lib)

########## Testing ##########
set(ATTA_SERVICE_LOCATOR_TEST_SOURCES
	tests/test.cpp
)
# Add to global test
atta_add_tests(${ATTA_SERVICE_LOCATOR_TEST_SOURCES})

# Create local test
atta_create_local_test(
	atta_service_locator_test
	"${ATTA_SERVICE_LOCATOR_TEST_SOURCES}"
	"atta_service_locator_lib"
)
