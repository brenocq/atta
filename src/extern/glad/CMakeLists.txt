cmake_minimum_required(VERSION 3.14)

if(ATTA_SYSTEM_NAME MATCHES "Web")
    set(GLAD_SOURCE
        src/gles2.c  # OpenGL for Embedded Systems
    )
else()
    set(GLAD_SOURCE
        src/egl.c    # Embedded-System Graphics Library
        src/gl.c     # OpenGL
        src/vulkan.c # Vulkan
    )
endif()

add_library(glad STATIC
	${GLAD_SOURCE}
)
target_include_directories(glad PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${ATTA_VERSION_SAFE}/extern/glad/include>
)
