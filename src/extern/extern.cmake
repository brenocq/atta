cmake_minimum_required(VERSION 3.12)

########## googletest ##########
if(ATTA_BUILD_TESTS)
    option(BUILD_GMOCK OFF)
    option(INSTALL_GTEST OFF)
    add_subdirectory(src/extern/googletest)
endif()

########## glfw ##########
if(NOT ATTA_SYSTEM_NAME MATCHES "Web")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory(src/extern/glfw)
    atta_add_libs(glfw)
    #atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)
endif()

########## glad ##########
add_subdirectory(src/extern/glad)
atta_add_libs(glad)

########## imgui ##########
set(IMGUI_SOURCE
	src/extern/imgui/imgui.cpp
	src/extern/imgui/imgui_demo.cpp
	src/extern/imgui/imgui_draw.cpp
	src/extern/imgui/imgui_tables.cpp
	src/extern/imgui/imgui_widgets.cpp
	src/extern/imgui/backends/imgui_impl_glfw.cpp
	src/extern/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC
	${IMGUI_SOURCE}
)
target_include_directories(imgui PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/extern/imgui>
    $<INSTALL_INTERFACE:extern/imgui>
)
target_link_libraries(imgui PRIVATE glfw)
if(NOT MSVC)
	target_compile_options(imgui PRIVATE -Wno-invalid-noreturn)
endif()
atta_add_libs(imgui)

########## assimp ##########
find_package(assimp)
set(ATTA_ASSIMP_SUPPORT FALSE)
set(ATTA_ASSIMP_TARGETS "")

if(assimp_FOUND)
    message(STATUS "[Extern] assimp supported (installed)")

    set(ATTA_ASSIMP_TARGETS ${ASSIMP_LIBRARIES})
    set(ATTA_ASSIMP_SUPPORT TRUE)
else()
    message(STATUS "[Extern] assimp supported (source)")

    set(BUILD_SHARED_LIBS FALSE CACHE INTERNAL "" FORCE)

    # Turn off assimp options to decrease build time and reduce size
    set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE CACHE INTERNAL "" FORCE)
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE INTERNAL "" FORCE)
    set(ASSIMP_BUILD_OBJ_IMPORTER TRUE CACHE INTERNAL "" FORCE)
    set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "" FORCE)
    set(ASSIMP_IGNORE_GIT_HASH ON CACHE INTERNAL "" FORCE)
    set(ASSIMP_NO_EXPORT ON CACHE INTERNAL "" FORCE)
    set(ASSIMP_BUILD_ZLIB ON CACHE INTERNAL "" FORCE)
    set(ASSIMP_INSTALL_PDB OFF CACHE INTERNAL "" FORCE )
    set(ASSIMP_INSTALL OFF CACHE INTERNAL "" FORCE)
  
    add_subdirectory(src/extern/assimp)
    set(ATTA_ASSIMP_TARGETS "assimp zlibstatic")

    set(ATTA_ASSIMP_SUPPORT TRUE)
endif()

if(NOT ATTA_ASSIMP_SUPPORT)
    message(WARNING "[Extern] assimp not supported")
endif()


########## stb_image ##########
atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/src/extern/stb_image)

########## imguizmo ##########
set(IMGUIZMO_SOURCE
    src/extern/imguizmo/ImGuizmo.cpp
)
add_library(imguizmo STATIC
    ${IMGUIZMO_SOURCE}
)
target_include_directories(imguizmo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/extern/imguizmo)
target_link_libraries(imguizmo PRIVATE imgui)
atta_add_include_dirs(${CMAKE_CURRENT_SOURCE_DIR}/src/extern/imguizmo)
atta_add_libs(imguizmo)

########## box2d ##########
find_package(box2d)
set(ATTA_BOX2D_SUPPORT FALSE)
set(ATTA_BOX2D_TARGETS "")

if(box2d_FOUND)
    message(STATUS "[Extern] box2d supported (installed)")

    set(ATTA_BOX2D_TARGETS box2d::box2d)

    set(ATTA_BOX2D_SUPPORT TRUE)
else()
    message(STATUS "[Extern] box2d supported (source)")

    set(BOX2D_BUILD_UNIT_TESTS OFF CACHE INTERNAL "" FORCE)
    set(BOX2D_BUILD_TESTBED OFF CACHE INTERNAL "" FORCE)
    add_subdirectory(src/extern/box2d)

    set(ATTA_BOX2D_TARGETS box2d)

    set(ATTA_BOX2D_SUPPORT TRUE)
endif()

if(NOT ATTA_BOX2D_SUPPORT)
    message(WARNING "[Extern] box2d not supported")
endif()

########## bullet3 ##########
find_package(Bullet QUIET)
set(ATTA_BULLET_SUPPORT FALSE)
set(ATTA_BULLET_TARGETS "")

if(Bullet_FOUND)
    message(STATUS "[Extern] bullet supported (installed)")

    set(ATTA_BULLET_TARGETS ${BULLET_LIBRARIES})
    include_directories(${BULLET_INCLUDE_DIR})

    set(ATTA_BULLET_SUPPORT TRUE)
else()
    message(STATUS "[Extern] bullet supported (source)")

    set(USE_GRAPHICAL_BENCHMARK OFF CACHE INTERNAL "" FORCE)
    set(BUILD_UNIT_TESTS OFF CACHE INTERNAL "" FORCE)
    set(INSTALL_CMAKE_FILES OFF CACHE INTERNAL "" FORCE)
    set(BUILD_BULLET3 OFF CACHE INTERNAL "" FORCE)
    set(BUILD_BULLET2_DEMOS OFF CACHE INTERNAL "" FORCE)
    set(BUILD_OBJ2SDF_EXTRA OFF CACHE INTERNAL "" FORCE)
    add_subdirectory(src/extern/bullet3)

    set(ATTA_BULLET_TARGETS "BulletDynamics BulletCollision LinearMath")
    target_include_directories(BulletDynamics PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/extern/bullet3/src>
        $<INSTALL_INTERFACE:extern/bullet3/src>
    )

    set(ATTA_BULLET_SUPPORT TRUE)
endif()

if(NOT ATTA_BULLET_SUPPORT)
    message(WARNING "[Extern] bullet not supported")
endif()

########## cpprestsdk ##########
find_package(cpprestsdk QUIET)
set(ATTA_CPPRESTSDK_SUPPORT FALSE)
set(ATTA_CPPRESTSDK_TARGETS "")

if(cpprestsdk_FOUND)
    message(STATUS "[Extern] cpprestsdk supported (installed)")

    set(OPENSSL_USE_STATIC_LIBS TRUE)
    find_package(OpenSSL)
    if(OpenSSL_FOUND)
        set(ATTA_CPPRESTSDK_TARGETS cpprestsdk::cpprest OpenSSL::Crypto)
        set(ATTA_CPPRESTSDK_SUPPORT TRUE)
    endif()
else()
    message(STATUS "[Extern] cpprestsdk supported (source)")

    set(CPPREST_INSTALL_HEADERS OFF CACHE BOOL "" FORCE)
    set(CPPREST_INSTALL OFF CACHE BOOL "" FORCE)
    set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(BUILD_SAMPLES OFF CACHE BOOL "" FORCE)

    add_subdirectory(src/extern/cpprestsdk)

    set(ATTA_CPPRESTSDK_TARGETS cpprest)
    target_include_directories(cpprest PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/extern/cpprestsdk/Release/include>
        $<INSTALL_INTERFACE:extern/cpprestsdk/Release/include>
    )

    set(ATTA_CPPRESTSDK_SUPPORT TRUE)
endif()

if(NOT ATTA_CPPRESTSDK_SUPPORT)
    message(WARNING "[Extern] cpprestsdk not supported")
endif()
