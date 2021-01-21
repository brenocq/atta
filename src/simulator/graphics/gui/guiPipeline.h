//--------------------------------------------------
// Robot Simulator
// guiPipeline.h
// Date: 2020-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef GUI_PIPELINE_H
#define GUI_PIPELINE_H

#include <vector>
#include <string>
#include "simulator/graphics/vulkan/device.h"
#include "simulator/graphics/vulkan/imageView.h"
#include "simulator/graphics/vulkan/shaderModule.h"
#include "simulator/graphics/vulkan/descriptorSetManager.h"
#include "guiRenderPass.h"
#include "guiFrameBuffer.h"
#include "guiPipelineLayout.h"
#include "guiUniformBuffer.h"
#include "widgets/widgets.h"
#include "font/fontLoader.h"

namespace atta
{
	class GuiPipeline
	{
		public:
			GuiPipeline(std::shared_ptr<vk::Device> device, 
					std::shared_ptr<Window> window,
					std::shared_ptr<vk::ImageView> imageView,
					std::shared_ptr<GuiUniformBuffer> uniformBuffer,
					std::shared_ptr<guib::FontLoader> _fontLoader);
			~GuiPipeline();

			void beginRender(VkCommandBuffer commandBuffer);
			void endRender(VkCommandBuffer commandBuffer);

			//---------- Getters and Setters ----------//
			VkPipeline handle() const { return _pipeline; }
			std::shared_ptr<GuiPipelineLayout> getPipelineLayout() const { return _pipelineLayout; }
			std::shared_ptr<vk::DescriptorSetManager> getDescriptorSetManager() const { return _descriptorSetManager; }
			std::shared_ptr<vk::DescriptorSets> getDescriptorSets() const { return _descriptorSetManager->getDescriptorSets(); }
			std::shared_ptr<vk::ImageView> getImageView() const { return _imageView; }
			std::shared_ptr<GuiFrameBuffer> getFrameBuffer() const { return _frameBuffer; }
			std::shared_ptr<GuiRenderPass> getRenderPass() const { return _renderPass; }

		private:
			VkPipeline _pipeline;
			std::shared_ptr<vk::Device> _device;
			std::shared_ptr<GuiFrameBuffer> _frameBuffer;
			std::shared_ptr<GuiRenderPass> _renderPass;

			std::shared_ptr<vk::ImageView> _imageView;
			VkFormat _imageFormat;
			VkExtent2D _imageExtent;

			std::shared_ptr<vk::ShaderModule> _vertShaderModule;
			std::shared_ptr<vk::ShaderModule> _fragShaderModule;
			std::shared_ptr<vk::DescriptorSetManager> _descriptorSetManager;
			std::shared_ptr<GuiPipelineLayout> _pipelineLayout;

			//---------- GiuB widget handler ----------//
			std::vector<guib::ClickDetectorArea> _clickableAreas;
	};
}
#endif// GUI_PIPELINE_H
