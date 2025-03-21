set(ATTA_GLFW_SUPPORT FALSE)
set(ATTA_GLFW_TARGETS "")

if(NOT ATTA_SYSTEM_NAME MATCHES "Web")
    #find_package(glfw3)
    #if(glfw3_FOUND)
    #    atta_add_libs(glfw)
    #    atta_log(Success Extern "GLFW support (installed)")
    #    set(ATTA_GLFW_SUPPORT TRUE)
    #    set(ATTA_GLFW_TARGETS glfw)
    #else()
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

        FetchContent_Declare(
            glfw
            GIT_REPOSITORY "https://github.com/glfw/glfw"
            GIT_TAG "3.3.8"
            GIT_PROGRESS TRUE
            GIT_SHALLOW TRUE
        )

        atta_log("Info" "Extern" "Fetching GLFW...")
        FetchContent_MakeAvailable(glfw)

        atta_log(Success Extern "GLFW support (source)")
        atta_add_include_dirs(${FETCHCONTENT_BASE_DIR}/glfw-src/include)
        atta_add_libs(glfw)
        set(ATTA_GLFW_SUPPORT TRUE)
        set(ATTA_GLFW_TARGETS glfw)
    #endif()
endif()
