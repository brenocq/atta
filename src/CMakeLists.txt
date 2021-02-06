set(src_files
	main.cpp
)

#---------- simulator
file(GLOB simulatorSrc
    "simulator/*.cpp"
)
set(src_files_simulator
	${simulatorSrc}
)

#---------- simulator/core
file(GLOB simulatorCoreSrc
    "simulator/core/*.cpp"
)
set(src_files_simulator_core
	${simulatorCoreSrc}
)

#---------- simulator/helpers
file(GLOB helpersSrc
    "simulator/helpers/*.cpp"
)
set(src_files_simulator_helpers
	${helpersSrc}
)

#---------- simulator/objects
file(GLOB objectsSrc
    "simulator/objects/*.cpp"
    "simulator/objects/**/*.cpp"
    "simulator/objects/**/**/*.cpp"
)

set(src_files_simulator_objects
	${objectsSrc}
)

#---------- simulator/physics
file(GLOB physicsSrc
    "simulator/physics/**/*.cpp"
    "simulator/physics/*.cpp"
)
set(src_files_simulator_physics
	${physicsSrc}
)

#---------- simulator/parallel
file(GLOB parallelSrc
    "simulator/parallel/**/*.cpp"
    "simulator/parallel/*.cpp"
)
set(src_files_simulator_parallel
	${parallelSrc}
)

#---------- simulator/graphics/core
file(GLOB src_files_simulator_graphics_core
    "simulator/graphics/core/*.cpp"
)

#---------- simulator/graphics/vulkan
file(GLOB src_files_simulator_graphics_vulkan
	"simulator/graphics/vulkan/*.cpp"
)

#---------- simulator/graphics/renderers
file(GLOB src_files_simulator_graphics_renderers
	"simulator/graphics/renderers/*.cpp"
	"simulator/graphics/renderers/rastRenderer/*.cpp"
	"simulator/graphics/renderers/rastRenderer/**/*.cpp"
)

#---------- simulator/graphics/rayTracing/rayTracingVulkan
file(GLOB src_files_simulator_graphics_raytracing_vulkan
    "simulator/graphics/rayTracing/rayTracingVulkan/*.cpp"
)

#---------- simulator/graphics/gui
file(GLOB src_files_simulator_graphics_gui
    "simulator/graphics/gui/**/*.cpp"
    "simulator/graphics/gui/*.cpp"
)

#---------- simulator/math
file(GLOB src_files_simulator_math
    "simulator/math/*.cpp"
)

include_directories(.)
include_directories(${glfw3_INCLUDE_DIRS})
include_directories(${glm_INCLUDE_DIRS})
include_directories(${Vulkan_INCLUDE_DIRS})
include_directories(${FREETYPE_INCLUDE_DIRS})
message("Vulkan library ${Vulkan_LIBRARY}")
link_directories(${Vulkan_LIBRARY})
add_subdirectory(shaders)
add_subdirectory(examples)

set(exe_name atta)
add_executable(${exe_name}
	${src_files} 
	${src_files_simulator} 
	${src_files_simulator_core} 
	${src_files_simulator_helpers} 
	${src_files_simulator_parallel}
	${src_files_simulator_math} 
	${src_files_simulator_objects} 
	${src_files_simulator_graphics_core} 
	${src_files_simulator_graphics_vulkan} 
	${src_files_simulator_graphics_renderers} 
	${src_files_simulator_graphics_gui} 
	${src_files_simulator_physics} 
	${src_files_simulator_graphics_raytracing_vulkan}
	#${src_files_examples}
)

if (UNIX)
	# GCC8 needs an extra lib for <filesystem>.
	# This is not needed with GCC9 or higher.
	#set(extra_libs -lbacktrace -lstdc++fs)
	set(extra_libs -lstdc++fs)
endif()

message("Vklibraries ${extra_libs}")
set_target_properties(${exe_name} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
target_link_libraries(${exe_name} PRIVATE glfw glm imgui::imgui tinyobjloader::tinyobjloader ${Vulkan_LIBRARIES} ${FREETYPE_LIBRARIES} ${extra_libs} attaLib)
add_dependencies(${exe_name} assets shaders)
