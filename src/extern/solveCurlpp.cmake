set(ATTA_CURLPP_SUPPORT FALSE)
set(ATTA_CURLPP_TARGETS "")

if(ATTA_SYSTEM_NAME MATCHES "Linux")
    set(CURLPP_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
    set(CURLPP_BUILD_EXAMPLES OFF CACHE INTERNAL "" FORCE)
    set(Uri_WARNINGS_AS_ERRORS OFF CACHE INTERNAL "" FORCE)

    FetchContent_Declare(
        curlpp
        GIT_REPOSITORY "https://github.com/jpbarrette/curlpp"
        GIT_TAG "236a1c5c116f68fd7c8ba9be5edbf492824e5295"
        GIT_PROGRESS TRUE
    )
    atta_log(Info Extern "Fetching cpp-netlib...")
    FetchContent_MakeAvailable(curlpp)

    atta_log(Success Extern "cpp-netlib supported (source)")
    set(ATTA_CURLPP_SUPPORT TRUE)
    set(ATTA_CURLPP_TARGETS curlpp_static)
endif()

if(ATTA_CURLPP_SUPPORT)
    list(APPEND ATTA_DEFINITIONS "ATTA_CURLPP_SUPPORT")
endif()
