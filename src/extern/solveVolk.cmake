set(ATTA_VOLK_SUPPORT FALSE)
set(ATTA_VOLK_TARGETS "")

if(ATTA_VULKAN_SUPPORT)
    FetchContent_Declare(
        volk
        GIT_REPOSITORY "https://github.com/zeux/volk.git"
        GIT_TAG "1.4.304"
        GIT_PROGRESS TRUE
        GIT_SHALLOW TRUE
    )
    atta_log(Info Extern "Fetching Volk...")
    FetchContent_MakeAvailable(volk)

    atta_add_libs(volk)

    atta_log(Success Extern "Volk support (source)")
    set(ATTA_VOLK_SUPPORT TRUE)
    set(ATTA_VOLK_TARGETS volk)
else()
    atta_log(Info Extern "Volk not supported (Vulkan is disabled)")
endif()
