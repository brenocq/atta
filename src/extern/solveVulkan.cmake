set(ATTA_VULKAN_SUPPORT FALSE)
set(ATTA_VULKAN_TARGETS "")

#find_package(Vulkan)

if(Vulkan_FOUND)
    set(ATTA_VULKAN_SUPPORT TRUE)
    set(ATTA_VULKAN_TARGETS ${Vulkan_LIBRARY})
    atta_add_include_dirs(${Vulkan_INCLUDE_DIRS})
    atta_log(Success Extern "Vulkan support (installed)")
else()
    atta_log(Warn Extern "Vulkan not supported")
endif()

if(ATTA_VULKAN_SUPPORT)
    atta_add_definition(ATTA_VULKAN_SUPPORT)
endif()
