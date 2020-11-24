set(exe_name ${MAIN_PROJECT})

set(src_files
	main.cpp
)

set(src_files_simulator
	simulator/object.cpp
	simulator/scene.cpp
	simulator/simulator.cpp
)

set(src_files_simulator_helpers
	simulator/helpers/debugDrawer.cpp
	simulator/helpers/drawHelper.cpp
	simulator/helpers/log.cpp
)

set(src_files_simulator_objects_basics
	simulator/objects/basics/box.cpp
	simulator/objects/basics/cylinder.cpp
	simulator/objects/basics/importedObject.cpp
	simulator/objects/basics/plane.cpp
	simulator/objects/basics/sphere.cpp
)

set(src_files_simulator_objects_controllers
)

set(src_files_simulator_objects_others
	simulator/objects/others/display/display.cpp
)

set(src_files_simulator_objects_sensors
	simulator/objects/sensors/camera/camera.cpp
)

set(src_files_simulator_physics
	simulator/physics/objectPhysics.cpp
	simulator/physics/physicsEngine.cpp
)

set(src_files_simulator_physics_colliders
)

set(src_files_simulator_physics_constraints
	simulator/physics/constraints/constraint.cpp
	simulator/physics/constraints/fixedConstraint.cpp
	simulator/physics/constraints/hingeConstraint.cpp
)

set(src_files_simulator_physics_forces
	simulator/physics/forces/force.cpp
	simulator/physics/forces/forceGenerator.cpp
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
	simulator/vulkan/pipeline/pipeline.cpp
	simulator/vulkan/pipeline/pipelineLayout.cpp
)

set(src_files_simulator_vulkan_ui
	simulator/vulkan/ui/uiFrameBuffer.cpp
	simulator/vulkan/ui/uiRenderPass.cpp
	simulator/vulkan/ui/userInterface.cpp
)

set(src_files_simulator_vulkan_ui_imgui
	simulator/vulkan/ui/imgui/imgui_impl_glfw.cpp
	simulator/vulkan/ui/imgui/imgui_impl_glfw.h
	simulator/vulkan/ui/imgui/imgui_impl_vulkan.cpp
	simulator/vulkan/ui/imgui/imgui_impl_vulkan.h
)


source_group("Main" FILES ${src_files})
source_group("Simulator" FILES ${src_files_simulator})
source_group("Simulator.Helpers" FILES ${src_files_simulator_helpers})
source_group("Simulator.Objects.Basics" FILES ${src_files_simulator_objects_basics})
source_group("Simulator.Objects.Others" FILES ${src_files_simulator_objects_others})
source_group("Simulator.Objects.Controllers" FILES ${src_files_simulator_objects_controllers})
source_group("Simulator.Objects.Sensors" FILES ${src_files_simulator_objects_sensors})
source_group("Simulator.Physics" FILES ${src_files_simulator_physics})
source_group("Simulator.Physics.Forces" FILES ${src_files_simulator_physics_forces})
source_group("Simulator.Physics.Colliders" FILES ${src_files_simulator_physics_colliders})
source_group("Simulator.Physics.Constraints" FILES ${src_files_simulator_physics_constraints})
source_group("Simulator.Vulkan" FILES ${src_files_simulator_vulkan})
source_group("Simulator.Vulkan.UI" FILES ${src_files_simulator_vulkan_ui})
source_group("Simulator.Vulkan.UI.Imgui" FILES ${src_files_simulator_vulkan_ui_imgui})
source_group("Simulator.Vulkan.RayTracing" FILES ${src_files_simulator_vulkan_raytracing})
source_group("Simulator.Vulkan.Pipeline" FILES ${src_files_simulator_vulkan_pipeline})

include_directories(.)
include_directories(${glfw3_INCLUDE_DIRS})
include_directories(${glm_INCLUDE_DIRS})
include_directories(${Vulkan_INCLUDE_DIRS})
message("Vulkan library ${Vulkan_LIBRARY}")
link_directories(${Vulkan_LIBRARY})
add_subdirectory(shaders)
add_subdirectory(examples)

add_executable(${exe_name} 
	${src_files} 
	${src_files_simulator} 
	${src_files_simulator_helpers} 
	${src_files_simulator_objects_basics} 
	${src_files_simulator_objects_others} 
	${src_files_simulator_objects_controllers} 
	${src_files_simulator_objects_sensors}
	${src_files_simulator_physics} 
	${src_files_simulator_physics_forces} 
	${src_files_simulator_physics_colliders} 
	${src_files_simulator_physics_constraints} 
	${src_files_simulator_vulkan} 
	${src_files_simulator_vulkan_ui} 
	${src_files_simulator_vulkan_ui_imgui} 
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
target_link_libraries(${exe_name} PRIVATE glfw glm imgui::imgui tinyobjloader::tinyobjloader ${Vulkan_LIBRARIES} ${extra_libs} robotSimLib)
add_dependencies(${exe_name} assets shaders)

