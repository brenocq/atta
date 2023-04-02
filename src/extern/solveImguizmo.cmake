set(ATTA_IMGUIZMO_SUPPORT FALSE)
set(ATTA_IMGUIZMO_TARGETS "")

FetchContent_Declare(
    imguizmo
    GIT_REPOSITORY "https://github.com/CedricGuillemet/ImGuizmo.git"
    GIT_TAG "1.83"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)
atta_log(Info Extern "Fetching ImGuizmo...")
FetchContent_MakeAvailable(imguizmo)
add_library(imguizmo STATIC ${FETCHCONTENT_BASE_DIR}/imguizmo-src/ImGuizmo.cpp)
target_link_libraries(imguizmo PRIVATE imgui)

atta_add_include_dirs(${FETCHCONTENT_BASE_DIR}/imguizmo-src)
atta_add_libs(imguizmo)

atta_log(Success Extern "ImGuizmo support (source)")
set(ATTA_IMGUIZMO_SUPPORT TRUE)
set(ATTA_IMGUIZMO_TARGETS imguizmo)
