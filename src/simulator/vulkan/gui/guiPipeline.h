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
#include "simulator/vulkan/device.h"
#include "simulator/vulkan/imageView.h"
#include "simulator/vulkan/shaderModule.h"
#include "simulator/vulkan/descriptorSetManager.h"
#include "guiRenderPass.h"
#include "guiFrameBuffer.h"
#include "guiPipelineLayout.h"
#include "guiUniformBuffer.h"

class GuiPipeline
{
	public:
		GuiPipeline(Device* device, 
				SwapChain* swapChain, 
				std::vector<GuiUniformBuffer*> uniformBuffers);
		~GuiPipeline();

		void beginRender(VkCommandBuffer commandBuffer, int imageIndex=0);
		void endRender(VkCommandBuffer commandBuffer);
		void render(VkCommandBuffer commandBuffer, int imageIndex=0);

		//---------- Getters and Setters ----------//
		VkPipeline handle() const { return _pipeline; }
		GuiPipelineLayout* getPipelineLayout() const { return _pipelineLayout; }
		DescriptorSetManager* getDescriptorSetManager() const { return _descriptorSetManager; }
		DescriptorSets* getDescriptorSets() const { return _descriptorSetManager->getDescriptorSets(); }
		std::vector<ImageView*> getImageViews() const { return _imageViews; }
		std::vector<GuiFrameBuffer*> getFrameBuffers() const { return _frameBuffers; }
		GuiRenderPass* getRenderPass() const { return _renderPass; }

	private:
		VkPipeline _pipeline;
		Device* _device;
		std::vector<GuiFrameBuffer*> _frameBuffers;
		GuiRenderPass* _renderPass;

		std::vector<ImageView*> _imageViews;
		VkFormat _imageFormat;
		VkExtent2D _imageExtent;

		ShaderModule* _vertShaderModule;
		ShaderModule* _fragShaderModule;
		DescriptorSetManager* _descriptorSetManager;
		GuiPipelineLayout* _pipelineLayout;
};

#endif// GUI_PIPELINE_H
