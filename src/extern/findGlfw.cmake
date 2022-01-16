set(ATTA_GLFW_SUPPORT FALSE)
set(ATTA_GLFW_TARGETS "")

if(NOT ATTA_SYSTEM_NAME MATCHES "Web")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        glfw
        GIT_REPOSITORY "https://github.com/glfw/glfw"
        GIT_TAG "df8d7bc892937a8b0f7c604c92a9f64f383cf48c"
        GIT_PROGRESS TRUE
    )

    atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/glfw-src/include)
    atta_add_libs(glfw)

    atta_log("Info" "Extern" "Fetching GLFW...")
    FetchContent_MakeAvailable(glfw)

    atta_log(Success Extern "GLFW support (source)")
    set(ATTA_GLFW_SUPPORT TRUE)
    set(ATTA_GLFW_TARGETS glfw)
endif()
