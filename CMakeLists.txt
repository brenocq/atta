cmake_minimum_required(VERSION 3.2)

project(RobotSimulator VERSION 0.1.0 LANGUAGES C CXX)

set(Vulkan_INCLUDE_DIR /home/breno/Programs/Vulkan/1.2.154.0/x86_64/include)
#set(ENV{VULKAN_SDK} /home/breno/Programs/Vulkan/1.2.154.0/x86_64)
#set(ENV{PATH} ENV{VULKAN_SDK}/bin:ENV{PATH})
#set(ENV{LD_LIBRARY_PATH} ENV{VULKAN_SDK}/lib:ENV{LD_LIBRARY_PATH})
#set(ENV{VK_LAYER_PATH} ENV{VULKAN_SDK}/etc/vulkan/explicit_layer.d)
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

foreach (outputconfig ${cmake_configuration_types})
    string(toupper ${outputconfig} outputconfig)
    set(cmake_runtime_output_directory_${outputconfig} ${project_binary_dir}/bin)
    set(cmake_library_output_directory_${outputconfig} ${project_binary_dir}/bin)
    set(cmake_archive_output_directory_${outputconfig} ${project_binary_dir}/lib)
endforeach()

if (UNIX)
	add_definitions(-DUNIX)
	add_compile_options("-std=c++17")
	add_compile_options("-Wall")
endif ()

#IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
	message("Removing otimizations from debug build")
	# Add -O0 to remove optimizations when using gcc
	#IF(CMAKE_COMPILER_IS_GNUCC)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
		set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
		#ENDIF(CMAKE_COMPILER_IS_GNUCC)
		#ENDIF()

find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_package(Vulkan REQUIRED)

message(STATUS "Searching Vulkan...")
IF (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
ELSE()
    message(STATUS ${Vulkan_LIBRARY})
ENDIF()

find_program(Vulkan_GLSLANG_VALIDATOR 
	NAMES glslangValidator 
	HINTS ENV VULKAN_SDK 
	PATH_SUFFIXES bin)
	
if (NOT Vulkan_GLSLANG_VALIDATOR)
    message(FATAL_ERROR "glslangValidator not found!")
endif()

set(MAIN_PROJECT "simulator")
add_subdirectory(assets)
add_subdirectory(lib)
add_subdirectory(src)
