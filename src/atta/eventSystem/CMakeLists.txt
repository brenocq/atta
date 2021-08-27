cmake_minimum_required(VERSION 3.12)

set(ATTA_EVENT_SYSTEM_SOURCE
	eventManager.cpp
)

add_library(atta_event_system_lib STATIC
	${ATTA_EVENT_SYSTEM_SOURCE}
)
atta_target_common(atta_event_system_lib)
atta_add_libs(atta_event_system_lib)

########## Testing ##########
set(ATTA_EVENT_SYSTEM_TEST_SOURCES
	tests/test.cpp
)
# Add to global test
atta_add_tests(${ATTA_EVENT_SYSTEM_TEST_SOURCES})

# Create local test
atta_create_local_test(
	atta_event_system_test
	"${ATTA_EVENT_SYSTEM_TEST_SOURCES}"
	"atta_event_system_lib"
)
