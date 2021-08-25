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
atta_add_tests("tests/test.cpp")
