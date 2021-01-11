set(exe_name ${MAIN_PROJECT})

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
#file(GLOB graphicsCoreSrc
#    "simulator/graphics/core/*.cpp"
#)
set(src_files_simulator_graphics_core
	simulator/graphics/core/model.cpp
	simulator/graphics/core/mesh.cpp
	simulator/graphics/core/material.cpp
	simulator/graphics/core/texture.cpp
	simulator/graphics/core/vertex.cpp
	simulator/graphics/core/window.cpp
)

#---------- simulator/graphics/vulkan
set(src_files_simulator_graphics_vulkan
	simulator/graphics/vulkan/vulkanCore.cpp
	simulator/graphics/vulkan/instance.cpp
	simulator/graphics/vulkan/debugCommon.cpp
	simulator/graphics/vulkan/debugMessenger.cpp
	simulator/graphics/vulkan/physicalDevice.cpp
	simulator/graphics/vulkan/device.cpp
)

#---------- simulator/graphics/vulkan
set(src_files_simulator_graphics_renderers
	simulator/graphics/renderers/renderer.cpp
	simulator/graphics/renderers/rastRenderer.cpp
)

#---------- simulator/vulkan
#file(GLOB VulkanSrc
#	"simulator/vulkan/*.cpp"
#)
#set(src_files_simulator_vulkan
#	${VulkanSrc}
#)
#
##---------- simulator/vulkan/rayTracing
#file(GLOB rtSrc
#    "simulator/vulkan/rayTracing/**/*.cpp"
#	"simulator/vulkan/rayTracing/*.cpp"
#)
#set(src_files_simulator_vulkan_raytracing
#	${rtSrc}
#)
#
#set(src_files_simulator_vulkan_pipeline
#	simulator/vulkan/pipeline/graphicsPipeline.cpp
#	simulator/vulkan/pipeline/linePipeline.cpp
#	simulator/vulkan/pipeline/outlinePipeline.cpp
#	simulator/vulkan/pipeline/skyboxPipeline.cpp
#	simulator/vulkan/pipeline/maskPipeline.cpp
#	simulator/vulkan/pipeline/pipeline.cpp
#	simulator/vulkan/pipeline/pipelineLayout.cpp
#)
#
## GUI
#file(GLOB guibSrc
#    "simulator/vulkan/gui/widgets/*.cpp"
#    "simulator/vulkan/gui/font/*.cpp"
#    "simulator/vulkan/gui/*.cpp"
#)
#set(src_files_simulator_vulkan_gui_widgets
#	${guibSrc}
#)

# Math
file(GLOB mathSrc
    "simulator/math/*.cpp"
)
set(src_files_simulator_math
	${mathSrc}
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
	#${src_files_simulator_physics} 
	#${src_files_simulator_vulkan_gui} 
	#${src_files_simulator_vulkan_gui_widgets} 
	#${src_files_simulator_vulkan_raytracing} 
	#${src_files_simulator_vulkan_pipeline} 
	#${src_files_examples}
)

if (UNIX)
	# GCC8 needs an extra lib for <filesystem>.
	# This is not needed with GCC9 or higher.
	#set(extra_libs -lbacktrace -lstdc++fs)
	set(extra_libs -lstdc++fs)
endif()

set_target_properties(${exe_name} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
target_link_libraries(${exe_name} PRIVATE glfw glm imgui::imgui tinyobjloader::tinyobjloader ${Vulkan_LIBRARIES} ${FREETYPE_LIBRARIES} ${extra_libs} robotSimLib)
add_dependencies(${exe_name} assets shaders)
