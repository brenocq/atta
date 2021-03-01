//--------------------------------------------------
// Atta Ray Tracing Vulkan
// rayTracing.h
// Date: 2020-07-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RT_VK_RAY_TRACING_H
#define ATTA_RT_VK_RAY_TRACING_H

#include <iostream>
#include <string.h>
#include <vector>

#include "defines.h"
#include "simulator/math/math.h"
#include "simulator/graphics/renderers/renderer.h"
#include "simulator/graphics/vulkan/commandPool.h"
#include "simulator/core/scene.h"
#include "deviceProcedures.h"
#include "rayTracingProperties.h"
#include "bottomLevelAccelerationStructure.h"
#include "topLevelAccelerationStructure.h"
#include "rayTracingPipeline.h"
#include "shaderBindingTable.h"
#include "uniformBuffer.h"

namespace atta::rt::vk
{
	class RayTracing : public Renderer
	{
		public:
			struct CreateInfo
			{
				std::shared_ptr<atta::vk::VulkanCore> vkCore;
				std::shared_ptr<atta::vk::CommandPool> commandPool;
				float width;
				float height;
				std::shared_ptr<Scene> scene;
				mat4 viewMat = atta::lookAt(vec3(2,2,0), vec3(0,0,0), vec3(0,1,0));
				mat4 projMat = atta::perspective(atta::radians(45.0), 1200.0/900, 0.01f, 1000.0f);
			};

			RayTracing(CreateInfo info);
			~RayTracing();

			void render(VkCommandBuffer commandBuffer);
			void updateCameraMatrix(mat4 viewMatrix);

			void recreateTLAS();

			//---------- Getters and Setters ----------//
			std::shared_ptr<atta::vk::Image> getAccumulationImage() const { return _accumulationImage; }

		private:
			void createPipeline();
			void createAccelerationStructures();
			void createBottomLevelStructures(VkCommandBuffer commandBuffer);
			void createTopLevelStructures(VkCommandBuffer commandBuffer);
			void createAccumulationImage();

			std::shared_ptr<atta::vk::Device> _device;
			std::shared_ptr<rt::vk::UniformBuffer> _uniformBuffer;
			std::shared_ptr<Scene> _scene;

			std::shared_ptr<RayTracingProperties> _rayTracingProperties;
			std::shared_ptr<DeviceProcedures> _deviceProcedures;

			// Ray Tracing objects
			std::vector<std::shared_ptr<BottomLevelAccelerationStructure>> _blas;
			std::vector<std::shared_ptr<TopLevelAccelerationStructure>> _tlas;

			// Ray Tracing pipeline
			std::shared_ptr<RayTracingPipeline> _rayTracingPipeline;
			std::shared_ptr<ShaderBindingTable> _shaderBindingTable;

			// Output images
			VkExtent2D _imageExtent;
			VkFormat _imageFormat;
			std::shared_ptr<atta::vk::Image> _accumulationImage;
			std::shared_ptr<atta::vk::ImageView> _accumulationImageView;
			
			// Ray tracing buffers
			std::shared_ptr<atta::vk::Buffer> _bottomBuffer;
			std::shared_ptr<atta::vk::Buffer> _bottomScratchBuffer;
			std::shared_ptr<atta::vk::Buffer> _topBuffer;
			std::shared_ptr<atta::vk::Buffer> _topScratchBuffer;
			std::shared_ptr<atta::vk::Buffer> _instancesBuffer;
	};
}

#endif// ATTA_RT_VK_RAY_TRACING_H
