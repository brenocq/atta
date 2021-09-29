cmake_minimum_required(VERSION 3.12)

set(ATTA_COMPUTER_VISION_SOURCES
	jpeg.cpp
)

add_library(atta_cv STATIC
	${ATTA_COMPUTER_VISION_SOURCES}
)
atta_target_common(atta_cv)
atta_add_libs(atta_cv)
target_link_libraries(atta_cv PUBLIC atta_core)
