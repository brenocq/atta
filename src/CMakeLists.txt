set(exe_name ${MAIN_PROJECT})

set(src_files
	main.cpp
)

set(src_files_simulator
	simulator/object.cpp
	simulator/scene.cpp
	simulator/simulator.cpp
)

file(GLOB helpersSrc
    "simulator/helpers/*.cpp"
)
set(src_files_simulator_helpers
	${helpersSrc}
)


file(GLOB objectsSrc
    "simulator/objects/*.cpp"
    "simulator/objects/**/*.cpp"
    "simulator/objects/**/**/*.cpp"
)

set(src_files_simulator_objects
	${objectsSrc}
)

#set(src_files_simulator_objects_controllers
#)
#
#set(src_files_simulator_objects_others
#	simulator/objects/others/display/display.cpp
#)
#
#set(src_files_simulator_objects_sensors
#	simulator/objects/sensors/camera/camera.cpp
#)

file(GLOB physicsSrc
    "simulator/physics/**/*.cpp"
    "simulator/physics/*.cpp"
)
set(src_files_simulator_physics
	${physicsSrc}
)


set(src_files_simulator_vulkan
	simulator/vulkan/application.cpp
	simulator/vulkan/buffer.cpp
	simulator/vulkan/colorBuffer.cpp
	simulator/vulkan/commandBuffers.cpp
	simulator/vulkan/commandPool.cpp
	simulator/vulkan/debugCommon.cpp
	simulator/vulkan/debugMessenger.cpp
	simulator/vulkan/depthBuffer.cpp
	simulator/vulkan/descriptorBinding.cpp
	simulator/vulkan/descriptorPool.cpp
	simulator/vulkan/descriptorSetLayout.cpp
	simulator/vulkan/descriptorSetManager.cpp
	simulator/vulkan/descriptorSets.cpp
	simulator/vulkan/device.cpp
	simulator/vulkan/fence.cpp
	simulator/vulkan/frameBuffer.cpp
	simulator/vulkan/helpers.cpp
	simulator/vulkan/image.cpp
	simulator/vulkan/imageMemoryBarrier.cpp	
	simulator/vulkan/imageView.cpp
	simulator/vulkan/indexBuffer.cpp
	simulator/vulkan/instance.cpp
	simulator/vulkan/material.cpp
	simulator/vulkan/model.cpp
	simulator/vulkan/modelViewController.cpp
	simulator/vulkan/physicalDevice.cpp
	simulator/vulkan/procedural.cpp
	simulator/vulkan/renderPass.cpp
	simulator/vulkan/sampler.cpp
	simulator/vulkan/semaphore.cpp
	simulator/vulkan/shaderModule.cpp
	simulator/vulkan/stagingBuffer.cpp
	simulator/vulkan/stbImage.cpp
	simulator/vulkan/surface.cpp
	simulator/vulkan/swapChain.cpp
	simulator/vulkan/texture.cpp
	simulator/vulkan/tinyObjLoader.cpp
	simulator/vulkan/uniformBuffer.cpp
	simulator/vulkan/vertex.cpp
	simulator/vulkan/vertexBuffer.cpp
	simulator/vulkan/vulkan.cpp
	simulator/vulkan/window.cpp
)

set(src_files_simulator_vulkan_raytracing
	simulator/vulkan/rayTracing/accelerationStructure.cpp
	simulator/vulkan/rayTracing/bottomLevelAccelerationStructure.cpp
	simulator/vulkan/rayTracing/deviceProcedures.cpp
	simulator/vulkan/rayTracing/rayTracing.cpp
	simulator/vulkan/rayTracing/rayTracingPipeline.cpp
	simulator/vulkan/rayTracing/shaderBindingTable.cpp
	simulator/vulkan/rayTracing/topLevelAccelerationStructure.cpp
)

set(src_files_simulator_vulkan_pipeline
	simulator/vulkan/pipeline/graphicsPipeline.cpp
	simulator/vulkan/pipeline/linePipeline.cpp
	simulator/vulkan/pipeline/outlinePipeline.cpp
	simulator/vulkan/pipeline/skyboxPipeline.cpp
	simulator/vulkan/pipeline/maskPipeline.cpp
	simulator/vulkan/pipeline/pipeline.cpp
	simulator/vulkan/pipeline/pipelineLayout.cpp
)

set(src_files_simulator_vulkan_gui
	simulator/vulkan/gui/guiFrameBuffer.cpp
	simulator/vulkan/gui/guiPipeline.cpp
	simulator/vulkan/gui/guiPipelineLayout.cpp
	simulator/vulkan/gui/guiRenderPass.cpp
	simulator/vulkan/gui/guiStructs.cpp
	simulator/vulkan/gui/guiUniformBuffer.cpp
	simulator/vulkan/gui/guiVertex.cpp
	simulator/vulkan/gui/guiRender.cpp
	simulator/vulkan/gui/userInterface.cpp
)

# GUI
file(GLOB guibSrc
    "simulator/vulkan/gui/widgets/*.cpp"
    "simulator/vulkan/gui/font/*.cpp"
)
set(src_files_simulator_vulkan_gui_widgets
	${guibSrc}
)

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
	${src_files_simulator_helpers} 
	${src_files_simulator_math} 
	${src_files_simulator_objects} 
	${src_files_simulator_physics} 
	${src_files_simulator_vulkan} 
	${src_files_simulator_vulkan_gui} 
	${src_files_simulator_vulkan_gui_widgets} 
	${src_files_simulator_vulkan_raytracing} 
	${src_files_simulator_vulkan_pipeline} 
	${src_files_examples}
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

