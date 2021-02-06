//--------------------------------------------------
// Atta Ray Tracing Vulkan
// rayTracingPipeline.h
// Date: 2020-07-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RT_VK_RAY_TRACING_PIPELINE_H
#define ATTA_RT_VK_RAY_TRACING_PIPELINE_H
#include <vector>
#include "simulator/core/scene.h"
#include "simulator/graphics/vulkan/uniformBuffer.h"
#include "simulator/graphics/vulkan/imageView.h"
#include "simulator/graphics/vulkan/pipelineLayout.h"
#include "simulator/graphics/vulkan/swapChain.h"
#include "simulator/graphics/vulkan/device.h"
#include "simulator/graphics/vulkan/descriptorSetManager.h"
#include "deviceProcedures.h"
#include "topLevelAccelerationStructure.h"

namespace atta::rt::vk
{
	class RayTracingPipeline final
	{
		public:
			RayTracingPipeline(
				std::shared_ptr<atta::vk::Device> device,
				std::shared_ptr<DeviceProcedures> deviceProcedures,
				std::shared_ptr<TopLevelAccelerationStructure> accelerationStructure,
				std::shared_ptr<atta::vk::ImageView> accumulationImageView,
				std::shared_ptr<atta::vk::ImageView> outputImageView,
				std::shared_ptr<atta::vk::UniformBuffer> uniformBuffer,
				std::shared_ptr<atta::vk::VulkanCore> vkCore);
			~RayTracingPipeline();

			uint32_t getRayGenShaderIndex() const { return _rayGenIndex; }
			uint32_t getMissShaderIndex() const { return _missIndex; }
			uint32_t getTriangleHitGroupIndex() const { return _triangleHitGroupIndex; }
			uint32_t getProceduralHitGroupIndex() const { return _proceduralHitGroupIndex; }

			VkDescriptorSet getDescriptorSet(uint32_t index) const;
			std::shared_ptr<atta::vk::PipelineLayout> getPipelineLayout() const { return _pipelineLayout; }
			VkPipeline handle() const { return _pipeline; }

		private:
			VkPipeline _pipeline;

			std::shared_ptr<atta::vk::DescriptorSetManager> _descriptorSetManager;
			std::shared_ptr<atta::vk::PipelineLayout> _pipelineLayout;
			std::shared_ptr<atta::vk::Device> _device;
			std::shared_ptr<atta::vk::VulkanCore> _vkCore;

			uint32_t _rayGenIndex;
			uint32_t _missIndex;
			uint32_t _triangleHitGroupIndex;
			uint32_t _proceduralHitGroupIndex;
	};
}

#endif// ATTA_RT_VK_RAY_TRACING_PIPELINE_H
