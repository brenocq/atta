set(ATTA_CPPRESTSDK_SUPPORT FALSE)
set(ATTA_CPPRESTSDK_TARGETS "")

find_package(cpprestsdk QUIET)

if(cpprestsdk_FOUND)
    set(OPENSSL_USE_STATIC_LIBS TRUE)
    find_package(OpenSSL)
    if(OpenSSL_FOUND)
        set(ATTA_CPPRESTSDK_TARGETS cpprestsdk::cpprest OpenSSL::Crypto)
        set(ATTA_CPPRESTSDK_SUPPORT TRUE)
        atta_log(Success Extern "Cpprestsdk supported (installed)")
    endif()
endif()

if(NOT ATTA_CPPRESTSDK_SUPPORT)
    set(CPPREST_INSTALL_HEADERS OFF CACHE BOOL "" FORCE)
    set(CPPREST_INSTALL OFF CACHE BOOL "" FORCE)
    set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        cpprestsdk
        GIT_REPOSITORY "https://github.com/microsoft/cpprestsdk.git"
        GIT_TAG "bfe34877914fd280512c97d41aaa99e88564fd8d"
        GIT_PROGRESS TRUE
    )
    atta_log(Info Extern "Fetching Cpprestsdk...")
    FetchContent_MakeAvailable(cpprestsdk)

    atta_log(Success Extern "Cpprestsdk supported (source)")
    set(ATTA_CPPRESTSDK_SUPPORT TRUE)
    set(ATTA_CPPRESTSDK_TARGETS cpprest)
endif()
