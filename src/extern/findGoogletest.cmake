if(ATTA_BUILD_TESTS)
    option(BUILD_GMOCK OFF)
    option(INSTALL_GTEST OFF)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY "https://github.com/google/googletest"
        GIT_TAG "4e9fe30884414b58d219d0170d39cbad96e5758a"
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(googletest)
endif()
