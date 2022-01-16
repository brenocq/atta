set(ATTA_BULLET_SUPPORT FALSE)
set(ATTA_BULLET_TARGETS "")

find_package(Bullet QUIET)
if(Bullet_FOUND)
    set(ATTA_BULLET_TARGETS ${BULLET_LIBRARIES})
    atta_add_include_dirs(${BULLET_INCLUDE_DIR})

    atta_log(Success Extern "Bullet support (installed)")
    set(ATTA_BULLET_SUPPORT TRUE)
else()
    set(USE_GRAPHICAL_BENCHMARK OFF CACHE INTERNAL "" FORCE)
    set(BUILD_UNIT_TESTS OFF CACHE INTERNAL "" FORCE)
    set(INSTALL_CMAKE_FILES OFF CACHE INTERNAL "" FORCE)
    set(BUILD_BULLET3 ON CACHE INTERNAL "" FORCE)
    set(BUILD_BULLET2_DEMOS OFF CACHE INTERNAL "" FORCE)
    set(BUILD_OBJ2SDF_EXTRA OFF CACHE INTERNAL "" FORCE)
    FetchContent_Declare(
        bullet3
        GIT_REPOSITORY "https://github.com/bulletphysics/bullet3"
        GIT_TAG "478da7469a34074aa051e8720734287ca371fd3e"
        GIT_PROGRESS TRUE
    )
    atta_log(Info Extern "Fetching Bullet...")
    FetchContent_MakeAvailable(bullet3)

    atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/bullet3-src/src)
    set(ATTA_BULLET_TARGETS "BulletDynamics;BulletCollision;LinearMath")
    atta_add_libs(${ATTA_BULLET_TARGETS})

    atta_log(Success Extern "Bullet support (source)")
    set(ATTA_BULLET_SUPPORT TRUE)
endif()
