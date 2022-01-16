set(ATTA_BOX2D_SUPPORT FALSE)
set(ATTA_BOX2D_TARGETS "")

find_package(box2d)
if(box2d_FOUND)
    atta_log(Success Extern "Box2D support (installed)")
    set(ATTA_BOX2D_TARGETS box2d::box2d)
    set(ATTA_BOX2D_SUPPORT TRUE)

    # Fix box2d include
    get_property(fix_box2d_includes TARGET ${ATTA_BOX2D_TARGETS} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
    list(REMOVE_ITEM fix_box2d_includes "/usr/local/src/extern")
    set_property(TARGET ${ATTA_BOX2D_TARGETS} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${fix_box2d_includes})
else()
    set(BOX2D_BUILD_UNIT_TESTS OFF CACHE INTERNAL "" FORCE)
    set(BOX2D_BUILD_TESTBED OFF CACHE INTERNAL "" FORCE)
    FetchContent_Declare(
        box2d
        GIT_REPOSITORY "https://github.com/erincatto/box2d"
        GIT_TAG "9dc24a6fd4f32442c4bcf80791de47a0a7d25afb"
        GIT_PROGRESS TRUE
    )
    atta_log(Info Extern "Fetching Box2D...")
    FetchContent_MakeAvailable(box2d)

    atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/box2d-src/include)
    atta_add_libs(box2d)

    atta_log(Success Extern "Box2D support (source)")
    set(ATTA_BOX2D_SUPPORT TRUE)
    set(ATTA_BOX2D_TARGETS box2d)
endif()
