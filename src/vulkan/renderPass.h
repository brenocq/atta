#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#include "swapChain.h"
#include "depthBuffer.h"

class RenderPass
{
	public:
		RenderPass(SwapChain* swapChain, DepthBuffer* depthBuffer, bool clearColorBuffer, bool clearDepthBuffer);
		~RenderPass();

		VkRenderPass handle() const { return _renderPass; }
		SwapChain* swapChain() const { return _swapChain; }
		DepthBuffer* depthBuffer() const { return _depthBuffer; }

	private:
		VkRenderPass _renderPass;
		SwapChain* _swapChain;
		DepthBuffer* _depthBuffer;
};

#endif// RENDER_PASS_H
