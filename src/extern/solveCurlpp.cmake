set(ATTA_CURLPP_SUPPORT FALSE)
set(ATTA_CURLPP_TARGETS "")

if(ATTA_SYSTEM_NAME MATCHES "Linux")
    # Fetch curl if not installed
    find_package(CURL)
    if(NOT CURL_FOUND)
        FetchContent_Declare(CURL GIT_REPOSITORY https://github.com/curl/curl.git)
        atta_log(Info Extern "Fetching curl... (curlpp dependency)")
        FetchContent_MakeAvailable(CURL)
    endif()

    FetchContent_Declare(
        curlpp
        GIT_REPOSITORY "https://github.com/jpbarrette/curlpp"
        GIT_TAG "236a1c5c116f68fd7c8ba9be5edbf492824e5295"
        GIT_PROGRESS TRUE
    )
    atta_log(Info Extern "Fetching curlpp...")
    FetchContent_MakeAvailable(curlpp)

    atta_log(Success Extern "curlpp supported (source)")
    set(ATTA_CURLPP_SUPPORT TRUE)
    set(ATTA_CURLPP_TARGETS curlpp_static)
endif()

if(ATTA_CURLPP_SUPPORT)
    list(APPEND ATTA_DEFINITIONS "ATTA_CURLPP_SUPPORT")
endif()
