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

#include <atta/math/math.h>
#include <atta/graphics/renderers/renderer.h>
#include <atta/graphics/vulkan/commandPool.h>
#include <atta/core/scene.h>
#include <atta/graphics/renderers/rayTracing/rayTracingVulkan/deviceProcedures.h>
#include <atta/graphics/renderers/rayTracing/rayTracingVulkan/rayTracingProperties.h>
#include <atta/graphics/renderers/rayTracing/rayTracingVulkan/bottomLevelAccelerationStructure.h>
#include <atta/graphics/renderers/rayTracing/rayTracingVulkan/topLevelAccelerationStructure.h>
#include <atta/graphics/renderers/rayTracing/rayTracingVulkan/rayTracingPipeline.h>
#include <atta/graphics/renderers/rayTracing/rayTracingVulkan/shaderBindingTable.h>
#include <atta/graphics/renderers/rayTracing/rayTracingVulkan/uniformBuffer.h>

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
				float fov;
				std::shared_ptr<Scene> scene;
				mat4 viewMat = atta::lookAt(vec3(2,2,0), vec3(0,0,0), vec3(0,1,0));
			};

			RayTracing(CreateInfo info);
			~RayTracing();

			void render(VkCommandBuffer commandBuffer);
			void updateCameraMatrix(mat4 viewMatrix);
			void resize(unsigned width, unsigned height);

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

			// Perspective projection matrix info
			float _fov;
	};
}

#endif// ATTA_RT_VK_RAY_TRACING_H
