cmake_minimum_required(VERSION 3.10)

project(Atta VERSION 0.0.1.1 LANGUAGES C CXX)

#set(CMAKE_DEBUG_POSTFIX d)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

#foreach (outputconfig ${cmake_configuration_types})
#    string(toupper ${outputconfig} outputconfig)
#    set(cmake_runtime_output_directory_${outputconfig} ${project_binary_dir}/bin)
#    set(cmake_library_output_directory_${outputconfig} ${project_binary_dir}/bin)
#    set(cmake_archive_output_directory_${outputconfig} ${project_binary_dir}/lib)
#endforeach()

#if(UNIX)
#	add_definitions(-DUNIX)
#	add_compile_options("-std=c++17")
#	add_compile_options("-Wall")
#	add_compile_options("-Wno-invalid-offsetof")
#endif ()

#IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
##message("Removing otimizations from debug build")
	# Add -O0 to remove optimizations when using gcc
	#IF(CMAKE_COMPILER_IS_GNUCC)
	##set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
	##set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
		#ENDIF(CMAKE_COMPILER_IS_GNUCC)
		#ENDIF()

#---------- Find packages ----------#
#find_package(glfw3 REQUIRED)
#find_package(tinyobjloader CONFIG REQUIRED)
#find_package(Freetype REQUIRED)
#find_package(Vulkan REQUIRED)

##---------- Search vulkan ----------#
#message(STATUS "Searching Vulkan...")
#if (NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
#		set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
#		STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
#endif()
#
#if(NOT Vulkan_FOUND)
#    message(FATAL_ERROR "Could not find Vulkan library!")
#else()
#	message(STATUS "Vulkan FOUND = ${Vulkan_FOUND}")
#	message(STATUS "Vulkan Include = ${Vulkan_INCLUDE_DIR}")
#	message(STATUS "Vulkan Lib = ${Vulkan_LIBRARY}")
#endif()
#
#find_program(Vulkan_GLSLANG_VALIDATOR 
#	NAMES glslangValidator 
#	HINTS ENV VULKAN_SDK 
#	PATH_SUFFIXES bin)
#	
#if (NOT Vulkan_GLSLANG_VALIDATOR)
#    message(FATAL_ERROR "glslangValidator not found!")
#endif()
#
#add_subdirectory(assets)
#add_subdirectory(lib)

#-------------- attacore -------------#

#---------- Package install ----------#
set(ATTA_LIB_LOCATION "lib/atta")
set(ATTA_INCLUDE_LOCATION "include/atta")
set(ATTA_EXECUTABLE_LOCATION "bin")

if(ATTA_GENERATE_PACKAGE_INSTALL)
	#---------- Install library and package ----------#
	set(attacore_SRCS
		# Core
		"src/atta/core/accelerator.cpp"
		"src/atta/core/common.cpp"
		"src/atta/core/robot.cpp"
		"src/atta/core/scene.cpp"
		# Helpers
		"src/atta/helpers/drawer.cpp"
		"src/atta/helpers/evaluator.cpp"
		"src/atta/helpers/log.cpp"
		# Math
		"src/atta/math/bounds.cpp"
		"src/atta/math/common.cpp"
		"src/atta/math/matrix.cpp"
		"src/atta/math/point.cpp"
		"src/atta/math/quaternion.cpp"
		"src/atta/math/ray.cpp"
		"src/atta/math/vector.cpp"
		# Objects
		"src/atta/objects/object.cpp"
		# Root
		"src/atta/atta.cpp"
		"src/atta/project.cpp"
	)

	set(attacore_HEADERS
		# Core
		"include/atta/core/accelerator.h"
		"include/atta/core/common.h"
		"include/atta/core/robot.h"
		"include/atta/core/scene.h"
		# Helpers
		"include/atta/helpers/drawer.h"
		"include/atta/helpers/evaluator.h"
		"include/atta/helpers/log.h"
		# Math
		"include/atta/math/bounds.h"
		"include/atta/math/common.h"
		"include/atta/math/math.h"
		"include/atta/math/matrix.h"
		"include/atta/math/point.h"
		"include/atta/math/quaternion.h"
		"include/atta/math/ray.h"
		"include/atta/math/vector.h"
		# Objects
		"include/atta/objects/object.h"
		# Root
		"include/atta/atta.h"
		"include/atta/project.h"
	)

	add_library(attacore ${attacore_HEADERS} ${attacore_SRCS})
	target_include_directories(attacore PUBLIC 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
		$<INSTALL_INTERFACE:${ATTA_INCLUDE_LOCATION}>
		$<INSTALL_INTERFACE:${ATTA_LIB_LOCATION}>
	)
	# Install libattacore.a (static library) to system lib directory
	install(TARGETS attacore EXPORT attacore-targets DESTINATION "${ATTA_LIB_LOCATION}")

	# Install header files to system include directory
	foreach( file ${attacore_HEADERS})
		get_filename_component(dir ${file} DIRECTORY)
		string(REPLACE "include/atta" ${ATTA_INCLUDE_LOCATION} dir_to_install ${dir})
		install(FILES ${file} DESTINATION ${dir_to_install} )
	endforeach()	

	# Create attacore-target config file inside system(atta) lib directory
	install(EXPORT attacore-targets DESTINATION "${ATTA_LIB_LOCATION}")

	# Create config file for find_package
	include(CMakePackageConfigHelpers)
	configure_package_config_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/AttaConfig.cmake.in"
		"${CMAKE_CURRENT_BINARY_DIR}/AttaConfig.cmake"
		INSTALL_DESTINATION	"${ATTA_LIB_LOCATION}"
		PATH_VARS ATTA_INCLUDE_LOCATION ATTA_LIB_LOCATION
	)

	# Create version file for find_package
	write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/AttaConfigVersion.cmake"
		VERSION ${PROJECT_VERSION}
		COMPATIBILITY SameMajorVersion
	)

	# Install config and version files to system lib directory
	install(FILES
		"${CMAKE_CURRENT_BINARY_DIR}/AttaConfig.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/AttaConfigVersion.cmake"
		DESTINATION "${ATTA_LIB_LOCATION}"
		COMPONENT Development
	)

	#---------- Generate executable ----------#
	add_executable(atta "${CMAKE_CURRENT_SOURCE_DIR}/src/attamain.cpp")
	target_link_libraries(atta attacore)
	# Install atta executable to system bin directory
	install(TARGETS atta RUNTIME DESTINATION ${ATTA_EXECUTABLE_LOCATION})
endif()
