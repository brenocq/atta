cmake_minimum_required(VERSION 3.10)

project(Atta VERSION 0.0.1.2 LANGUAGES C CXX)

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

if(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -Wall -Wno-invalid-offsetof")
endif()

#IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
##message("Removing otimizations from debug build")
	# Add -O0 to remove optimizations when using gcc
	#IF(CMAKE_COMPILER_IS_GNUCC)
	##set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
	##set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")
		#ENDIF(CMAKE_COMPILER_IS_GNUCC)
		#ENDIF()

		#message(STATUS "Vulkan FOUND = ${Vulkan_FOUND}")
		#message(STATUS "Vulkan Include = ${Vulkan_INCLUDE_DIR}")
		#message(STATUS "Vulkan Lib = ${Vulkan_LIBRARY}")
		#link_directories(${Vulkan_LIBRARY})
#---------- Find packages ----------#
find_package(glfw3 REQUIRED)
#find_package(tinyobjloader CONFIG REQUIRED)
find_package(Freetype REQUIRED)
# Help: If vulkan was not found please check the environment variable VULKAN_SDK
# Make sure that this variable is defined to the right path
find_package(Vulkan REQUIRED)

#---------- Search vulkan ----------#
if(NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
		set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
		STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
endif()

if(NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find Vulkan library!")
else()
	message(STATUS "Found vulkan include at ${Vulkan_INCLUDE_DIR}")
endif()

find_program(Vulkan_GLSLANG_VALIDATOR 
	NAMES glslangValidator 
	HINTS ENV VULKAN_SDK 
	PATH_SUFFIXES bin)
	
if (NOT Vulkan_GLSLANG_VALIDATOR)
	message(FATAL_ERROR "Could not find glslangValidator!")
endif()

add_subdirectory(src/shaders)
#add_subdirectory(lib)

#---------- Package install ----------#
set(ATTA_LIB_LOCATION "lib/atta")
set(ATTA_INCLUDE_LOCATION "include/atta")
set(ATTA_EXECUTABLE_LOCATION "bin")

if(ATTA_GENERATE_PACKAGE_INSTALL)
	#---------- Install library and package ----------#
	set(attacore_SRCS
		# algorithms/
			# cv/
			"src/atta/algorithms/cv/bundleAdjustment/bundleAdjustment.cpp"
			# imgProc/
			"src/atta/algorithms/imgProc/fundamentalMatrix.cpp"
			"src/atta/algorithms/imgProc/homography.cpp"
			# linAlg/
				# elimination/
				"src/atta/algorithms/linAlg/elimination/gaussianElimination.cpp"
				"src/atta/algorithms/linAlg/elimination/gaussJordanFull.cpp"
				# LU/
				"src/atta/algorithms/linAlg/LU/LU.cpp"
				# QR/
				"src/atta/algorithms/linAlg/QR/QR.cpp"
				# SVD/
				"src/atta/algorithms/linAlg/SVD/SVD.cpp"
		# communication/
			# socket/
			"src/atta/communication/socket/client.cpp"
			"src/atta/communication/socket/server.cpp"
		# core/
		"src/atta/core/accelerator.cpp"
		"src/atta/core/common.cpp"
		"src/atta/core/robot.cpp"
		"src/atta/core/scene.cpp"
		# graphics/core/
		"src/atta/graphics/core/light.cpp"
		"src/atta/graphics/core/material.cpp"
		"src/atta/graphics/core/mesh.cpp"
		"src/atta/graphics/core/model.cpp"
		"src/atta/graphics/core/modelViewController.cpp"
		"src/atta/graphics/core/objectInfo.cpp"
		"src/atta/graphics/core/texture.cpp"
		"src/atta/graphics/core/vertex.cpp"
		"src/atta/graphics/core/window.cpp"
		# graphics/gui/
			# graphics/gui/font/
			"src/atta/graphics/gui/font/fontLoader.cpp"
			# graphics/gui/widgets/
            "src/atta/graphics/gui/widgets/align.cpp"
            "src/atta/graphics/gui/widgets/box.cpp"
            "src/atta/graphics/gui/widgets/button.cpp"
            "src/atta/graphics/gui/widgets/buttonText.cpp"
			"src/atta/graphics/gui/widgets/buttonImage.cpp"
            "src/atta/graphics/gui/widgets/clickDetector.cpp"
            "src/atta/graphics/gui/widgets/column.cpp"
            "src/atta/graphics/gui/widgets/draggable.cpp"
            "src/atta/graphics/gui/widgets/image.cpp"
            "src/atta/graphics/gui/widgets/menuButton.cpp"
			"src/atta/graphics/gui/widgets/menuItem.cpp"
            "src/atta/graphics/gui/widgets/padding.cpp"
            "src/atta/graphics/gui/widgets/protect.cpp"
            "src/atta/graphics/gui/widgets/row.cpp"
            "src/atta/graphics/gui/widgets/text.cpp"
            "src/atta/graphics/gui/widgets/topBar.cpp"
            "src/atta/graphics/gui/widgets/visibility.cpp"
            "src/atta/graphics/gui/widgets/widget.cpp"
            "src/atta/graphics/gui/widgets/widgetStructs.cpp"
            "src/atta/graphics/gui/widgets/window.cpp"
    	"src/atta/graphics/gui/guiFrameBuffer.cpp"
        "src/atta/graphics/gui/guiPipeline.cpp"
        "src/atta/graphics/gui/guiPipelineLayout.cpp"
        "src/atta/graphics/gui/guiRender.cpp"
        "src/atta/graphics/gui/guiRenderPass.cpp"
        "src/atta/graphics/gui/guiState.cpp"
        "src/atta/graphics/gui/guiStructs.cpp"
        "src/atta/graphics/gui/guiUniformBuffer.cpp"
        "src/atta/graphics/gui/guiVertex.cpp"
        "src/atta/graphics/gui/userInterface.cpp"
		# graphics/renderers
		"src/atta/graphics/renderers/renderer.cpp"
			# graphics/renderers/rastRenderer
			"src/atta/graphics/renderers/rastRenderer/pipelines/graphicsPipeline.cpp"
			"src/atta/graphics/renderers/rastRenderer/pipelines/linePipeline.cpp"
			"src/atta/graphics/renderers/rastRenderer/pipelines/maskPipeline.cpp"
			"src/atta/graphics/renderers/rastRenderer/pipelines/outlinePipeline.cpp"
			"src/atta/graphics/renderers/rastRenderer/pipelines/pointPipeline.cpp"
			"src/atta/graphics/renderers/rastRenderer/pipelines/skyboxPipeline.cpp"
			"src/atta/graphics/renderers/rastRenderer/rastRenderer.cpp"
			# graphics/renderers/rayTracing/rayTracingCPU/
		  	"src/atta/graphics/renderers/rayTracing/rayTracingCPU/cameras/camera.cpp"
        	"src/atta/graphics/renderers/rayTracing/rayTracingCPU/cameras/perspectiveCamera.cpp"
            "src/atta/graphics/renderers/rayTracing/rayTracingCPU/geometry/surfaceInteraction.cpp"
            "src/atta/graphics/renderers/rayTracing/rayTracingCPU/rayTracing.cpp"
			# graphics/renderers/rayTracing/rayTracingVulkan/
            "src/atta/graphics/renderers/rayTracing/rayTracingVulkan/accelerationStructure.cpp"
            "src/atta/graphics/renderers/rayTracing/rayTracingVulkan/bottomLevelAccelerationStructure.cpp"
            "src/atta/graphics/renderers/rayTracing/rayTracingVulkan/bottomLevelGeometry.cpp"
            "src/atta/graphics/renderers/rayTracing/rayTracingVulkan/deviceProcedures.cpp"
            "src/atta/graphics/renderers/rayTracing/rayTracingVulkan/rayTracing.cpp"
            "src/atta/graphics/renderers/rayTracing/rayTracingVulkan/rayTracingPipeline.cpp"
            "src/atta/graphics/renderers/rayTracing/rayTracingVulkan/rayTracingProperties.cpp"
            "src/atta/graphics/renderers/rayTracing/rayTracingVulkan/shaderBindingTable.cpp"
            "src/atta/graphics/renderers/rayTracing/rayTracingVulkan/topLevelAccelerationStructure.cpp"
            "src/atta/graphics/renderers/rayTracing/rayTracingVulkan/uniformBuffer.cpp"
			# graphics/renderers/renderer2D/
			"src/atta/graphics/renderers/renderer2D/pipelines/graphicsPipeline2D.cpp"
			"src/atta/graphics/renderers/renderer2D/pipelines/linePipeline.cpp"
			"src/atta/graphics/renderers/renderer2D/renderer2D.cpp"
			"src/atta/graphics/renderers/renderer2D/uniformBuffer2D.cpp"
		# graphics/vulkan
		"src/atta/graphics/vulkan/buffer.cpp"
        "src/atta/graphics/vulkan/colorBuffer.cpp"
        "src/atta/graphics/vulkan/commandBuffers.cpp"
        "src/atta/graphics/vulkan/commandPool.cpp"
        "src/atta/graphics/vulkan/debugCommon.cpp"
        "src/atta/graphics/vulkan/debugMessenger.cpp"
        "src/atta/graphics/vulkan/depthBuffer.cpp"
        "src/atta/graphics/vulkan/descriptorBinding.cpp"
        "src/atta/graphics/vulkan/descriptorPool.cpp"
        "src/atta/graphics/vulkan/descriptorSetLayout.cpp"
        "src/atta/graphics/vulkan/descriptorSetManager.cpp"
        "src/atta/graphics/vulkan/descriptorSets.cpp"
        "src/atta/graphics/vulkan/device.cpp"
        "src/atta/graphics/vulkan/fence.cpp"
        "src/atta/graphics/vulkan/frameBuffer.cpp"
        "src/atta/graphics/vulkan/helpers.cpp"
        "src/atta/graphics/vulkan/image.cpp"
        "src/atta/graphics/vulkan/imageMemoryBarrier.cpp"
        "src/atta/graphics/vulkan/imageView.cpp"
        "src/atta/graphics/vulkan/indexBuffer.cpp"
        "src/atta/graphics/vulkan/instance.cpp"
        "src/atta/graphics/vulkan/material.cpp"
        "src/atta/graphics/vulkan/model.cpp"
        "src/atta/graphics/vulkan/physicalDevice.cpp"
        "src/atta/graphics/vulkan/pipeline.cpp"
        "src/atta/graphics/vulkan/pipelineLayout.cpp"
        "src/atta/graphics/vulkan/procedural.cpp"
        "src/atta/graphics/vulkan/renderPass.cpp"
        "src/atta/graphics/vulkan/sampler.cpp"
        "src/atta/graphics/vulkan/semaphore.cpp"
        "src/atta/graphics/vulkan/shaderModule.cpp"
        "src/atta/graphics/vulkan/stagingBuffer.cpp"
        "src/atta/graphics/vulkan/surface.cpp"
        "src/atta/graphics/vulkan/swapChain.cpp"
        "src/atta/graphics/vulkan/texture.cpp"
        "src/atta/graphics/vulkan/uniformBuffer.cpp"
        "src/atta/graphics/vulkan/vertexBuffer.cpp"
        "src/atta/graphics/vulkan/vulkan.cpp"
        "src/atta/graphics/vulkan/vulkanCore.cpp"
		# helpers
		"src/atta/helpers/drawer.cpp"
		"src/atta/helpers/evaluator.cpp"
		"src/atta/helpers/log.cpp"
		# math
		"src/atta/math/bounds.cpp"
		"src/atta/math/common.cpp"
		"src/atta/math/matrix.cpp"
		"src/atta/math/point.cpp"
		"src/atta/math/quaternion.cpp"
		"src/atta/math/ray.cpp"
		# objects
		"src/atta/objects/object.cpp"
        "src/atta/objects/basics/box.cpp"
        "src/atta/objects/basics/cylinder.cpp"
        "src/atta/objects/basics/halfSpace.cpp"
        "src/atta/objects/basics/importedObject.cpp"
        "src/atta/objects/basics/plane.cpp"
        "src/atta/objects/basics/sphere.cpp"
        "src/atta/objects/lights/distant.cpp"
        "src/atta/objects/lights/infinite.cpp"
        "src/atta/objects/lights/point.cpp"
        "src/atta/objects/lights/spot.cpp"
		"src/atta/objects/lights/triangleMesh.cpp"
        "src/atta/objects/others/display/display.cpp"
        "src/atta/objects/sensors/camera/camera.cpp"
		# parallel
		"src/atta/parallel/barrier.cpp"
		"src/atta/parallel/threadManager.cpp"
		"src/atta/parallel/worker.cpp"
		"src/atta/parallel/workerGeneralist.cpp"
		"src/atta/parallel/workerGui.cpp"
		# peripheral
		"src/atta/peripheral/camera.cpp"
		# physics
		"src/atta/physics/constraints/constraint.cpp"
		"src/atta/physics/constraints/fixedConstraint.cpp"
		"src/atta/physics/constraints/hingeConstraint.cpp"
		"src/atta/physics/contacts/contact.cpp"
		"src/atta/physics/contacts/contactGenerator.cpp"
		"src/atta/physics/contacts/contactResolver.cpp"
		"src/atta/physics/forces/anchoredSpringForce.cpp"
		"src/atta/physics/forces/dragForce.cpp"
		"src/atta/physics/forces/force.cpp"
		"src/atta/physics/forces/forceGenerator.cpp"
		"src/atta/physics/forces/gravityForce.cpp"
		"src/atta/physics/forces/springForce.cpp"
		"src/atta/physics/shapes/boxShape.cpp"
		"src/atta/physics/shapes/capsuleShape.cpp"
		"src/atta/physics/shapes/cylinderShape.cpp"
		"src/atta/physics/shapes/halfSpaceShape.cpp"
		"src/atta/physics/shapes/planeShape.cpp"
		"src/atta/physics/shapes/shape.cpp"
		"src/atta/physics/shapes/sphereShape.cpp"
		"src/atta/physics/body.cpp"
		"src/atta/physics/physicsEngine.cpp"
		# root
		"src/atta/atta.cpp"
		"src/atta/project.cpp"
	)

	set(attacore_HEADERS
		# algorithms/
			# cv/
			"include/atta/algorithms/cv/bundleAdjustment/bundleAdjustment.h"
			# imgProc/
			"include/atta/algorithms/imgProc/fundamentalMatrix.h"
			"include/atta/algorithms/imgProc/homography.h"
			# linAlg/
				# elimination/
				"include/atta/algorithms/linAlg/elimination/gaussianElimination.h"
				"include/atta/algorithms/linAlg/elimination/gaussJordanFull.h"
				# LU/
				"include/atta/algorithms/linAlg/LU/LU.h"
				# QR/
				"include/atta/algorithms/linAlg/QR/QR.h"
				# SVD/
				"include/atta/algorithms/linAlg/SVD/SVD.h"
		# communication/
			# socket/
			"include/atta/communication/socket/client.h"
			"include/atta/communication/socket/server.h"
		# core
		"include/atta/core/accelerator.h"
		"include/atta/core/common.h"
		"include/atta/core/robot.h"
		"include/atta/core/scene.h"
		# extern
		"include/atta/extern/stb_image.h"
		"include/atta/extern/stbImage.h"
		"include/atta/extern/stbImage.cpp"
		"include/atta/extern/tiny_obj_loader.h"
		"include/atta/extern/tinyObjLoader.h"
		# graphics/core
		"include/atta/graphics/core/light.h"
		"include/atta/graphics/core/material.h"
		"include/atta/graphics/core/mesh.h"
		"include/atta/graphics/core/model.h"
		"include/atta/graphics/core/modelViewController.h"
		"include/atta/graphics/core/objectInfo.h"
		"include/atta/graphics/core/texture.h"
		"include/atta/graphics/core/vertex.h"
		"include/atta/graphics/core/window.h"
		# graphics/gui/
			# graphics/gui/font/
			"include/atta/graphics/gui/font/fontLoader.h"
			"include/atta/graphics/gui/font/fontStructs.h"
			# graphics/gui/widgets/
            "include/atta/graphics/gui/widgets/align.h"
            "include/atta/graphics/gui/widgets/box.h"
            "include/atta/graphics/gui/widgets/button.h"
            "include/atta/graphics/gui/widgets/buttonText.h"
			"include/atta/graphics/gui/widgets/buttonImage.h"
            "include/atta/graphics/gui/widgets/clickDetector.h"
            "include/atta/graphics/gui/widgets/column.h"
            "include/atta/graphics/gui/widgets/draggable.h"
            "include/atta/graphics/gui/widgets/image.h"
            "include/atta/graphics/gui/widgets/menuButton.h"
			"include/atta/graphics/gui/widgets/menuItem.h"
            "include/atta/graphics/gui/widgets/padding.h"
            "include/atta/graphics/gui/widgets/protect.h"
            "include/atta/graphics/gui/widgets/row.h"
            "include/atta/graphics/gui/widgets/text.h"
            "include/atta/graphics/gui/widgets/topBar.h"
            "include/atta/graphics/gui/widgets/visibility.h"
            "include/atta/graphics/gui/widgets/widget.h"
            "include/atta/graphics/gui/widgets/widgets.h"
            "include/atta/graphics/gui/widgets/widgetStructs.h"
            "include/atta/graphics/gui/widgets/window.h"
        "include/atta/graphics/gui/guiFrameBuffer.h"
        "include/atta/graphics/gui/guiPipeline.h"
        "include/atta/graphics/gui/guiPipelineLayout.h"
        "include/atta/graphics/gui/guiRender.h"
        "include/atta/graphics/gui/guiRenderPass.h"
        "include/atta/graphics/gui/guiState.h"
        "include/atta/graphics/gui/guiStructs.h"
        "include/atta/graphics/gui/guiUniformBuffer.h"
        "include/atta/graphics/gui/guiVertex.h"
        "include/atta/graphics/gui/userInterface.h"
		# graphics/renderers
		"include/atta/graphics/renderers/renderer.h"
			# graphics/renderers/rastRenderer
			"include/atta/graphics/renderers/rastRenderer/pipelines/graphicsPipeline.h"
			"include/atta/graphics/renderers/rastRenderer/pipelines/linePipeline.h"
			"include/atta/graphics/renderers/rastRenderer/pipelines/maskPipeline.h"
			"include/atta/graphics/renderers/rastRenderer/pipelines/outlinePipeline.h"
			"include/atta/graphics/renderers/rastRenderer/pipelines/pointPipeline.h"
			"include/atta/graphics/renderers/rastRenderer/pipelines/skyboxPipeline.h"
			"include/atta/graphics/renderers/rastRenderer/rastRenderer.h"
			# graphics/renderers/rayTracing/rayTracingCPU/
		  	"include/atta/graphics/renderers/rayTracing/rayTracingCPU/cameras/camera.h"
        	"include/atta/graphics/renderers/rayTracing/rayTracingCPU/cameras/cameras.h"
        	"include/atta/graphics/renderers/rayTracing/rayTracingCPU/cameras/perspectiveCamera.h"
            "include/atta/graphics/renderers/rayTracing/rayTracingCPU/geometry/surfaceInteraction.h"
            "include/atta/graphics/renderers/rayTracing/rayTracingCPU/rayTracing.h"
			# graphics/renderers/rayTracing/rayTracingVulkan/
            "include/atta/graphics/renderers/rayTracing/rayTracingVulkan/accelerationStructure.h"
            "include/atta/graphics/renderers/rayTracing/rayTracingVulkan/bottomLevelAccelerationStructure.h"
            "include/atta/graphics/renderers/rayTracing/rayTracingVulkan/bottomLevelGeometry.h"
            "include/atta/graphics/renderers/rayTracing/rayTracingVulkan/deviceProcedures.h"
            "include/atta/graphics/renderers/rayTracing/rayTracingVulkan/rayTracing.h"
            "include/atta/graphics/renderers/rayTracing/rayTracingVulkan/rayTracingPipeline.h"
            "include/atta/graphics/renderers/rayTracing/rayTracingVulkan/rayTracingProperties.h"
            "include/atta/graphics/renderers/rayTracing/rayTracingVulkan/shaderBindingTable.h"
            "include/atta/graphics/renderers/rayTracing/rayTracingVulkan/topLevelAccelerationStructure.h"
            "include/atta/graphics/renderers/rayTracing/rayTracingVulkan/uniformBuffer.h"
			# graphics/renderers/renderer2D/
			"include/atta/graphics/renderers/renderer2D/pipelines/graphicsPipeline2D.h"
			"include/atta/graphics/renderers/renderer2D/pipelines/linePipeline.h"
			"include/atta/graphics/renderers/renderer2D/renderer2D.h"
			"include/atta/graphics/renderers/renderer2D/uniformBuffer2D.h"
		# graphics/vulkan
		"include/atta/graphics/vulkan/buffer.h"
        "include/atta/graphics/vulkan/colorBuffer.h"
        "include/atta/graphics/vulkan/commandBuffers.h"
        "include/atta/graphics/vulkan/commandPool.h"
        "include/atta/graphics/vulkan/debugCommon.h"
        "include/atta/graphics/vulkan/debugMessenger.h"
        "include/atta/graphics/vulkan/depthBuffer.h"
        "include/atta/graphics/vulkan/descriptorBinding.h"
        "include/atta/graphics/vulkan/descriptorPool.h"
        "include/atta/graphics/vulkan/descriptorSetLayout.h"
        "include/atta/graphics/vulkan/descriptorSetManager.h"
        "include/atta/graphics/vulkan/descriptorSets.h"
        "include/atta/graphics/vulkan/device.h"
        "include/atta/graphics/vulkan/fence.h"
        "include/atta/graphics/vulkan/frameBuffer.h"
        "include/atta/graphics/vulkan/helpers.h"
        "include/atta/graphics/vulkan/image.h"
        "include/atta/graphics/vulkan/imageMemoryBarrier.h"
        "include/atta/graphics/vulkan/imageView.h"
        "include/atta/graphics/vulkan/indexBuffer.h"
        "include/atta/graphics/vulkan/instance.h"
        "include/atta/graphics/vulkan/material.h"
        "include/atta/graphics/vulkan/model.h"
        "include/atta/graphics/vulkan/physicalDevice.h"
        "include/atta/graphics/vulkan/pipeline.h"
        "include/atta/graphics/vulkan/pipelineLayout.h"
        "include/atta/graphics/vulkan/procedural.h"
        "include/atta/graphics/vulkan/renderPass.h"
        "include/atta/graphics/vulkan/sampler.h"
        "include/atta/graphics/vulkan/semaphore.h"
        "include/atta/graphics/vulkan/shaderModule.h"
        "include/atta/graphics/vulkan/stagingBuffer.h"
        "include/atta/graphics/vulkan/surface.h"
        "include/atta/graphics/vulkan/swapChain.h"
        "include/atta/graphics/vulkan/texture.h"
        "include/atta/graphics/vulkan/uniformBuffer.h"
        "include/atta/graphics/vulkan/vertexBuffer.h"
        "include/atta/graphics/vulkan/vulkan.h"
        "include/atta/graphics/vulkan/vulkanCore.h"
		# helpers
		"include/atta/helpers/drawer.h"
		"include/atta/helpers/evaluator.h"
		"include/atta/helpers/log.h"
		# math
		"include/atta/math/bounds.h"
		"include/atta/math/common.h"
		"include/atta/math/math.h"
		"include/atta/math/matrix.cpp"
		"include/atta/math/matrix.h"
		"include/atta/math/point.h"
		"include/atta/math/quaternion.h"
		"include/atta/math/ray.h"
		"include/atta/math/vector.cpp"
		"include/atta/math/vector.h"
		# objects
		"include/atta/objects/object.h"
        "include/atta/objects/basics/basics.h"
        "include/atta/objects/basics/box.h"
        "include/atta/objects/basics/cylinder.h"
        "include/atta/objects/basics/halfSpace.h"
        "include/atta/objects/basics/importedObject.h"
        "include/atta/objects/basics/plane.h"
        "include/atta/objects/basics/sphere.h"
        "include/atta/objects/lights/distant.h"
        "include/atta/objects/lights/infinite.h"
        "include/atta/objects/lights/lights.h"
        "include/atta/objects/lights/point.h"
        "include/atta/objects/lights/spot.h"
		"include/atta/objects/lights/triangleMesh.h"
        "include/atta/objects/others/display/display.h"
        "include/atta/objects/sensors/camera/camera.h"
		# parallel
		"include/atta/parallel/barrier.h"
		"include/atta/parallel/threadManager.h"
		"include/atta/parallel/worker.h"
		"include/atta/parallel/workerGeneralist.h"
		"include/atta/parallel/workerGui.h"
		# peripheral
		"include/atta/peripheral/camera.h"
		# physics
		"include/atta/physics/constraints/constraint.h"
		"include/atta/physics/constraints/constraints.h"
		"include/atta/physics/constraints/fixedConstraint.h"
		"include/atta/physics/constraints/hingeConstraint.h"
		"include/atta/physics/contacts/contact.h"
		"include/atta/physics/contacts/contactGenerator.h"
		"include/atta/physics/contacts/contactResolver.h"
		"include/atta/physics/forces/anchoredSpringForce.h"
		"include/atta/physics/forces/dragForce.h"
		"include/atta/physics/forces/force.h"
		"include/atta/physics/forces/forceGenerator.h"
		"include/atta/physics/forces/gravityForce.h"
		"include/atta/physics/forces/springForce.h"
		"include/atta/physics/shapes/boxShape.h"
		"include/atta/physics/shapes/capsuleShape.h"
		"include/atta/physics/shapes/cylinderShape.h"
		"include/atta/physics/shapes/halfSpaceShape.h"
		"include/atta/physics/shapes/planeShape.h"
		"include/atta/physics/shapes/shape.h"
		"include/atta/physics/shapes/shapes.h"
		"include/atta/physics/shapes/sphereShape.h"
		"include/atta/physics/body.h"
		"include/atta/physics/physicsEngine.h"
		# root
		"include/atta/atta.h"
		"include/atta/project.h"
	)

	set(attacore_SHADERS
		# gui
		"include/atta/assets/shaders/gui/guiShader.frag.spv"
        "include/atta/assets/shaders/gui/guiShader.vert.spv"
		# rastRenderer/
			# rastRenderer/graphics
			"include/atta/assets/shaders/rastRenderer/graphics/graphicsShader.frag.spv"
			"include/atta/assets/shaders/rastRenderer/graphics/graphicsShader.vert.spv"
			# rastRenderer/line
			"include/atta/assets/shaders/rastRenderer/line/lineShader.frag.spv"
			"include/atta/assets/shaders/rastRenderer/line/lineShader.vert.spv"
			# rastRenderer/mask
			"include/atta/assets/shaders/rastRenderer/mask/maskShader.frag.spv"
			"include/atta/assets/shaders/rastRenderer/mask/maskShader.vert.spv"
			# rastRenderer/outline
			"include/atta/assets/shaders/rastRenderer/outline/outlineShader.frag.spv"
			"include/atta/assets/shaders/rastRenderer/outline/outlineShader.vert.spv"
			# rastRenderer/point
			"include/atta/assets/shaders/rastRenderer/point/pointShader.frag.spv"
			"include/atta/assets/shaders/rastRenderer/point/pointShader.vert.spv"
			# rastRenderer/skybox
			"include/atta/assets/shaders/rastRenderer/skybox/skyboxShader.frag.spv"
			"include/atta/assets/shaders/rastRenderer/skybox/skyboxShader.vert.spv"
		# rayTracing
        "include/atta/assets/shaders/rayTracing/rayTracing.rchit.spv"
        "include/atta/assets/shaders/rayTracing/rayTracing.rgen.spv"
        "include/atta/assets/shaders/rayTracing/rayTracing.rmiss.spv"
        "include/atta/assets/shaders/rayTracing/rayTracingShadow.rmiss.spv"
		# renderer2D
			# renderer2D/line
			"include/atta/assets/shaders/renderer2D/line/lineShader.frag.spv"
			"include/atta/assets/shaders/renderer2D/line/lineShader.vert.spv"
		"include/atta/assets/shaders/renderer2D/renderer2D.frag.spv"
        "include/atta/assets/shaders/renderer2D/renderer2D.vert.spv"
	)

	set(attacore_TEXTURES
		"include/atta/assets/textures/icon.png"
		"include/atta/assets/icons/logo16.png"
		"include/atta/assets/icons/logo50.png"
		"include/atta/assets/icons/move100.png"
		"include/atta/assets/icons/rotate100.png"
		"include/atta/assets/icons/scale100.png"
		"include/atta/assets/textures/WinterForest_Env.hdr"
		"include/atta/assets/textures/WinterForest_Ref.hdr"
		"include/atta/assets/textures/attaIBL/ibl_brdf_lut.png"
		"include/atta/assets/fonts/Ubuntu/Ubuntu-Medium.ttf"
		"include/atta/assets/fonts/Roboto/Roboto-Thin.ttf"
		"include/atta/assets/fonts/Roboto/Roboto-Light.ttf"
		"include/atta/assets/fonts/Roboto/Roboto-Regular.ttf"
		"include/atta/assets/fonts/Roboto/Roboto-Medium.ttf"
	)

	add_library(attacore ${attacore_HEADERS} ${attacore_SRCS})

	# Include directories
	target_include_directories(attacore PUBLIC ${Vulkan_INCLUDE_DIRS})
	target_include_directories(attacore PUBLIC ${FREETYPE_INCLUDE_DIRS})
	target_link_libraries(attacore PRIVATE glfw ${Vulkan_LIBRARIES} ${FREETYPE_LIBRARIES})
	#tinyobjloader::tinyobjloader  
	add_dependencies(attacore shaders)
	target_include_directories(attacore PUBLIC 
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
		$<INSTALL_INTERFACE:${ATTA_INCLUDE_LOCATION}>
		$<INSTALL_INTERFACE:${ATTA_LIB_LOCATION}>
	)
	# Install libattacore.a (static library) to system lib directory
	install(TARGETS attacore EXPORT attacore-targets DESTINATION "${ATTA_LIB_LOCATION}")

	# Install header files to system include directory
	foreach( file ${attacore_HEADERS};${attacore_SHADERS};${attacore_TEXTURES})
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
	add_executable(atta "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
	target_link_libraries(atta attacore)
	## Install atta executable to system bin directory
	install(TARGETS atta RUNTIME DESTINATION ${ATTA_EXECUTABLE_LOCATION})
endif()
