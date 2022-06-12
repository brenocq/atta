set(ATTA_IMGUIZMO_SUPPORT FALSE)
set(ATTA_IMGUIZMO_TARGETS "")

FetchContent_Declare(
    imguizmo
    GIT_REPOSITORY "https://github.com/CedricGuillemet/ImGuizmo.git"
    GIT_TAG "a7ec0dd3b9e7dbd253c8ca891bbf621d82e3c804"
    GIT_PROGRESS TRUE
)
atta_log(Info Extern "Fetching ImGuizmo...")
FetchContent_MakeAvailable(imguizmo)
add_library(imguizmo STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/imguizmo-src/ImGuizmo.cpp
)
#target_include_directories(imguizmo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/imguizmo-src)
#target_include_directories(imguizmo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/imgui-src)
target_link_libraries(imguizmo PRIVATE imgui)

atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/imguizmo-src)
atta_add_libs(imguizmo)

atta_log(Success Extern "ImGuizmo support (source)")
set(ATTA_IMGUIZMO_SUPPORT TRUE)
set(ATTA_IMGUIZMO_TARGETS imguizmo)
