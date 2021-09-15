cmake_minimum_required(VERSION 3.12)

set(ATTA_EVENT_SYSTEM_SOURCE
	event.cpp
	eventManager.cpp
)

add_library(atta_event_system STATIC
	${ATTA_EVENT_SYSTEM_SOURCE}
)
atta_target_common(atta_event_system)
atta_add_libs(atta_event_system)

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
	"atta_event_system"
)
