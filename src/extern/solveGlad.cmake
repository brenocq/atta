add_subdirectory(src/extern/glad)
atta_add_libs(glad)
atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/src/extern/glad/include)

set(ATTA_GLAD_SUPPORT TRUE)
set(ATTA_GLAD_TARGETS glad)

set(GLAD_SOURCE
	src/glad.c
)
