set(ATTA_ASSIMP_SUPPORT FALSE)
set(ATTA_ASSIMP_TARGETS "")

find_package(assimp)
if(assimp_FOUND AND FALSE)
    atta_log(Success Extern "Assimp support (installed)")
    set(ATTA_ASSIMP_TARGETS ${ASSIMP_LIBRARIES})
    set(ATTA_ASSIMP_SUPPORT TRUE)
else()
    set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE CACHE INTERNAL "" FORCE)
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "" FORCE)
    set(ASSIMP_BUILD_OBJ_IMPORTER TRUE CACHE INTERNAL "" FORCE)
    set(ASSIMP_BUILD_FBX_IMPORTER TRUE CACHE INTERNAL "" FORCE)
    set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
    set(ASSIMP_IGNORE_GIT_HASH ON CACHE INTERNAL "" FORCE)
    set(ASSIMP_NO_EXPORT ON CACHE INTERNAL "" FORCE)
    set(ASSIMP_BUILD_ZLIB ON CACHE INTERNAL "" FORCE)
    set(ASSIMP_INSTALL_PDB OFF CACHE INTERNAL "" FORCE )
    set(ASSIMP_INSTALL OFF CACHE INTERNAL "" FORCE)
    FetchContent_Declare(
        assimp
        GIT_REPOSITORY "https://github.com/assimp/assimp"
        GIT_TAG "5976d5443739714f4d8fe8a4cc5a041c8bb5a967"
        GIT_PROGRESS TRUE
    )
    atta_log(Info Extern "Fetching Assimp...")
    FetchContent_MakeAvailable(assimp)

    atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/assimp-src/include)
    atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/assimp-build/include)
    atta_add_libs(assimp)

    atta_log(Success Extern "Assimp support (source)")
    set(ATTA_ASSIMP_SUPPORT TRUE)
    set(ATTA_ASSIMP_TARGETS assimp)
endif()
