set(ATTA_BULLET_SUPPORT FALSE)
set(ATTA_BULLET_TARGETS "")

#if(Bullet_FOUND)
#    set(ATTA_BULLET_TARGETS ${BULLET_LIBRARIES})
#    atta_add_include_dirs(${BULLET_INCLUDE_DIR})
#
#    atta_log(Success Extern "Bullet support (installed)")
#    set(ATTA_BULLET_SUPPORT TRUE)
#else()
    set(USE_GRAPHICAL_BENCHMARK OFF CACHE INTERNAL "" FORCE)
    set(BUILD_UNIT_TESTS OFF CACHE INTERNAL "" FORCE)
    set(BUILD_BULLET3 ON CACHE INTERNAL "" FORCE)
    set(BUILD_OBJ2SDF_EXTRA OFF CACHE INTERNAL "" FORCE)
    set(BUILD_BULLET2_DEMOS OFF CACHE INTERNAL "" FORCE)
    set(BUILD_CPU_DEMOS OFF CACHE INTERNAL "" FORCE)
    set(BUILD_EXTRAS OFF CACHE INTERNAL "" FORCE)
    set(INSTALL_LIBS OFF CACHE INTERNAL "" FORCE)
    set(INSTALL_CMAKE_FILES OFF CACHE INTERNAL "" FORCE)

    FetchContent_Declare(
        bullet3
        URL "https://storage.googleapis.com/atta-deps/bullet3-3.24-light.zip"
    )
    atta_log(Info Extern "Fetching Bullet...")
    FetchContent_MakeAvailable(bullet3)

    atta_add_include_dirs(
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/_deps/bullet3-src/src>
        $<INSTALL_INTERFACE:include/${ATTA_VERSION_SAFE}/extern/bullet3/src>
    )

    set(ATTA_BULLET_TARGETS "BulletDynamics;BulletCollision;LinearMath")
    atta_add_libs(${ATTA_BULLET_TARGETS})

    atta_log(Success Extern "Bullet support (source)")
    set(ATTA_BULLET_SUPPORT TRUE)
#endif()
