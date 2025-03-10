set(ATTA_SHADERC_SUPPORT FALSE)
set(ATTA_SHADERC_TARGETS "")

if(ATTA_VULKAN_SUPPORT)
    atta_log(Info Extern "Fetching SPIRV-Headers (SPIRV-Tools dependency)...")
    FetchContent_Declare(
        spirv-headers
        GIT_REPOSITORY "https://github.com/KhronosGroup/SPIRV-Headers.git"
        GIT_TAG "54a521dd130ae1b2f38fef79b09515702d135bdd"
        GIT_PROGRESS TRUE
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(spirv-headers)

    atta_log(Info Extern "Fetching SPIRV-Tools (Shaderc dependency)...")
    FetchContent_Declare(
        spirv-tools
        GIT_REPOSITORY "https://github.com/KhronosGroup/SPIRV-Tools.git"
        GIT_TAG "9e470ef0f95d6923fa19dc5c5dc48c1442eb5a8b"
        GIT_PROGRESS TRUE
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(spirv-tools)

    atta_log(Info Extern "Fetching glslang (Shaderc dependency)...")
    FetchContent_Declare(
        glslang
        GIT_REPOSITORY "https://github.com/KhronosGroup/glslang.git"
        GIT_TAG "vulkan-sdk-1.4.304.1"
        GIT_PROGRESS TRUE
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(glslang)

    atta_log(Info Extern "Fetching Shaderc...")
    FetchContent_Declare(
        shaderc
        GIT_REPOSITORY "https://github.com/google/shaderc.git"
        GIT_TAG "v2025.1"
        GIT_PROGRESS TRUE
        GIT_SHALLOW TRUE
    )

    set(SHADERC_SKIP_INSTALL ON)
    set(SHADERC_SKIP_TESTS ON)
    set(SHADERC_SKIP_EXAMPLES ON)
    FetchContent_MakeAvailable(shaderc)

    atta_add_libs(shaderc_combined)
    atta_add_include_dirs(${FETCHCONTENT_BASE_DIR}/shaderc-src/libshaderc/include)

    atta_log(Success Extern "Shaderc support (source)")
    set(ATTA_SHADERC_SUPPORT TRUE)
    set(ATTA_SHADERC_TARGETS shaderc_combined)
else()
    atta_log(Info Extern "Shaderc not supported (Vulkan is disabled)")
endif()
