set(ATTA_CURLPP_SUPPORT FALSE)
set(ATTA_CURLPP_TARGETS "")

if(ATTA_SYSTEM_NAME MATCHES "Linux")
    find_package(CURL)
    if(CURL_FOUND)
        FetchContent_Declare(
            curlpp
            GIT_REPOSITORY "https://github.com/jpbarrette/curlpp"
            GIT_TAG "d080e301ec71061c46049d23fedfb8106be3153f"
            GIT_PROGRESS TRUE
        )
        atta_log(Info Extern "Fetching curlpp...")
        atta_FetchContent_MakeAvailable(curlpp)

        # Fix INTERFACE_INCLUDE_DIRECTORIES to not include local build
        set_property(TARGET curlpp_static PROPERTY INTERFACE_INCLUDE_DIRECTORIES "")

        atta_log(Success Extern "curlpp supported (source)")
        set(ATTA_CURLPP_SUPPORT TRUE)
        set(ATTA_CURLPP_TARGETS curlpp_static)
        atta_add_libs(${ATTA_CURLPP_TARGETS})
    endif()
endif()

if(ATTA_CURLPP_SUPPORT)
    atta_add_definition(ATTA_CURLPP_SUPPORT)
endif()
