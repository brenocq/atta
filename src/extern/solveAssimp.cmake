set(ATTA_ASSIMP_SUPPORT FALSE)
set(ATTA_ASSIMP_TARGETS "")

#find_package(assimp)
## TODO work with installed assimp
#if(assimp_FOUND AND FALSE)
#    atta_log(Success Extern "Assimp support (installed)")
#
#    set(ATTA_ASSIMP_TARGETS ${ASSIMP_LIBRARIES})
#    set(ATTA_ASSIMP_SUPPORT TRUE)
#
#    # TODO Fix assimp include (not working yet, directory must exist to compile)
#    #get_property(fix_assimp_includes TARGET ${ATTA_ASSIMP_TARGETS} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
#    #list(REMOVE_ITEM fix_assimp_includes "/usr/local/src/extern")
#    #set_property(TARGET ${ATTA_ASSIMP_TARGETS} PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${fix_assimp_includes})
#else()
    set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE CACHE INTERNAL "" FORCE)
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "" FORCE)
    set(ASSIMP_BUILD_OBJ_IMPORTER TRUE CACHE INTERNAL "" FORCE)
    set(ASSIMP_BUILD_FBX_IMPORTER TRUE CACHE INTERNAL "" FORCE)
    set(ASSIMP_BUILD_STL_IMPORTER TRUE CACHE INTERNAL "" FORCE)
    set(ASSIMP_BUILD_PLY_IMPORTER TRUE CACHE INTERNAL "" FORCE)
    set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
    set(ASSIMP_IGNORE_GIT_HASH ON CACHE INTERNAL "" FORCE)
    set(ASSIMP_NO_EXPORT ON CACHE INTERNAL "" FORCE)
    set(ASSIMP_BUILD_ZLIB ON CACHE INTERNAL "" FORCE)
    set(ASSIMP_INSTALL_PDB OFF CACHE INTERNAL "" FORCE )
    set(ASSIMP_INSTALL OFF CACHE INTERNAL "" FORCE)
    FetchContent_Declare(
        assimp
        URL "https://storage.googleapis.com/atta-deps/assimp-5.2.4-light.zip"
    )
    atta_log(Info Extern "Fetching Assimp...")
    atta_FetchContent_MakeAvailable(assimp) 

    atta_add_include_dirs(${CMAKE_BINARY_DIR}/_deps/assimp-src/include)
    atta_add_include_dirs(${CMAKE_BINARY_DIR}/_deps/assimp-build/include)
    atta_add_libs(assimp)

    atta_log(Success Extern "Assimp support (source)")
    set(ATTA_ASSIMP_SUPPORT TRUE)
    set(ATTA_ASSIMP_TARGETS assimp)
#endif()
