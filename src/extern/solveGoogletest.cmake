if(ATTA_BUILD_TESTS)
    option(BUILD_GMOCK ON)
    option(INSTALL_GTEST OFF)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY "https://github.com/google/googletest"
        GIT_TAG "release-1.12.1"
        GIT_PROGRESS TRUE
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(googletest)
    atta_add_include_dirs(${FETCHCONTENT_BASE_DIR}/googletest-src/googlemock/include/)
    atta_add_include_dirs(${FETCHCONTENT_BASE_DIR}/googletest-src/googletest/include/)
endif()
