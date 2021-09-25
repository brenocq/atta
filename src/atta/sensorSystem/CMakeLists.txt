cmake_minimum_required(VERSION 3.12)

set(ATTA_SENSOR_SYSTEM_SOURCE
	sensorManager.cpp
)

add_library(atta_sensor_system STATIC
	${ATTA_SENSOR_SYSTEM_SOURCE}
)
target_link_libraries(atta_sensor_system PRIVATE atta_graphics_system)
atta_target_common(atta_sensor_system)
atta_add_libs(atta_sensor_system)
