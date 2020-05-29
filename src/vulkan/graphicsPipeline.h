#ifndef GRAPHICS_PIPELINE_H
#define GRAPHICS_PIPELINE_H

#include <vector>
#include "swapChain.h"
#include "depthBuffer.h"
#include "descriptorSetManager.h"
#include "pipelineLayout.h"
#include "renderPass.h"
#include "../assets/scene.h"
#include "../assets/uniformBuffer.h"

class GraphicsPipeline
{
	public:
		GraphicsPipeline(
				SwapChain* swapChain, 
				DepthBuffer* depthBuffer,
				const std::vector<UniformBuffer>& uniformBuffers,
				Scene* scene,
				bool isWireFrame);
		~GraphicsPipeline();

		VkDescriptorSet descriptorSet(uint32_t index) const;

		VkPipeline handle() const { return _pipeline; }
		bool isWireFrame() const { return _isWireFrame; }
		PipelineLayout* pipelineLayout() const { return _pipelineLayout; }
		RenderPass* renderPass() const { return _renderPass; }

	private:
		SwapChain* _swapChain;
		const bool _isWireFrame;
		VkPipeline _pipeline;

		DescriptorSetManager* _descriptorSetManager;
		PipelineLayout* _pipelineLayout;
		RenderPass* _renderPass;
};

#endif// GRAPHICS_PIPELINE_H
