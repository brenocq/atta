set(ATTA_CPPNETLIB_SUPPORT FALSE)
set(ATTA_CPPNETLIB_TARGETS "")

if(NOT (ATTA_SYSTEM_NAME MATCHES "Web"))
    set(CPP-NETLIB_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
    set(CPP-NETLIB_BUILD_EXAMPLES OFF CACHE INTERNAL "" FORCE)
    set(Uri_WARNINGS_AS_ERRORS OFF CACHE INTERNAL "" FORCE)

    FetchContent_Declare(
        cppnetlib
        GIT_REPOSITORY "https://github.com/cpp-netlib/cpp-netlib.git"
        GIT_TAG "96a087a1a68d276d9ffa8fc474e78f691ab73bf9"
        GIT_PROGRESS TRUE
    )
    atta_log(Info Extern "Fetching cpp-netlib...")
    FetchContent_MakeAvailable(cppnetlib)

    atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/cppnetlib-src)
    atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/cppnetlib-src/deps/uri/include)

    atta_log(Success Extern "cpp-netlib supported (source)")
    set(ATTA_CPPNETLIB_SUPPORT TRUE)
    set(ATTA_CPPNETLIB_TARGETS ${CPPNETLIB_LIBRARIES})
endif()
if(ATTA_CPPNETLIB_SUPPORT)
    list(APPEND ATTA_DEFINITIONS "ATTA_CPPNETLIB_SUPPORT")
endif()
