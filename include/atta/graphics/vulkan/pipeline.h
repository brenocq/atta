//--------------------------------------------------
// Robot Simulator
// pipeline.h
// Date: 2020-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VULKAN_PIPELINE_H
#define ATTA_GRAPHICS_VULKAN_PIPELINE_H

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <string>

#include <atta/graphics/vulkan/pipelineLayout.h>
#include <atta/graphics/vulkan/device.h>
#include <atta/graphics/vulkan/vulkanCore.h>
#include <atta/graphics/vulkan/shaderModule.h>
#include <atta/graphics/vulkan/imageView.h>
#include <atta/graphics/vulkan/frameBuffer.h>
#include <atta/graphics/vulkan/descriptorSetManager.h>
#include <atta/graphics/vulkan/descriptorBinding.h>
#include <atta/graphics/core/vertex.h>
#include <atta/core/scene.h>

namespace atta::vk
{
	class Pipeline
	{
		public:
			Pipeline(std::shared_ptr<VulkanCore> vkCore, 
					std::vector<std::shared_ptr<ImageView>> imageViews,
					std::shared_ptr<Scene> scene);
			virtual ~Pipeline();

			virtual void render(VkCommandBuffer commandBuffer, int imageIndex=0) = 0;

			//---------- Getters and Setters ----------//
			VkPipeline handle() const { return _pipeline; }
			std::shared_ptr<PipelineLayout> getPipelineLayout() const { return _pipelineLayout; }
			std::shared_ptr<DescriptorSetManager> getDescriptorSetManager() const { return _descriptorSetManager; }
			std::shared_ptr<DescriptorSets> getDescriptorSets() const { return _descriptorSetManager->getDescriptorSets(); }
			std::vector<std::shared_ptr<ImageView>> getImageViews() const { return _imageViews; }
			std::vector<std::shared_ptr<FrameBuffer>> getFrameBuffers() const { return _frameBuffers; }
			std::shared_ptr<RenderPass> getRenderPass() const { return _renderPass; }

		protected:
			VkPipeline _pipeline;
			std::shared_ptr<VulkanCore> _vkCore;
			std::shared_ptr<Device> _device;
			std::vector<std::shared_ptr<ImageView>> _imageViews;
			std::vector<std::shared_ptr<FrameBuffer>> _frameBuffers;
			std::shared_ptr<RenderPass> _renderPass;
			std::shared_ptr<Scene> _scene;

			std::shared_ptr<PipelineLayout> _pipelineLayout;
			std::shared_ptr<DescriptorSetManager> _descriptorSetManager;
			std::shared_ptr<ShaderModule> _vertShaderModule;
			std::shared_ptr<ShaderModule> _fragShaderModule;

			// Output image info
			VkExtent2D _imageExtent;
			VkFormat _imageFormat;
	};
}

#endif// ATTA_GRAPHICS_VULKAN_PIPELINE_H
