//--------------------------------------------------
// Atta Graphics 2D Renderer
// renderer2D.h
// Date: 2021-02-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_2D_RENDERER_H
#define ATTA_GRAPHICS_2D_RENDERER_H

#include "simulator/graphics/renderers/renderer.h"
#include "pipelines/graphicsPipeline2D.h"
#include "uniformBuffer2D.h"
#include "simulator/core/scene.h"

namespace atta
{
	class Renderer2D : public Renderer
	{
		public:
			struct CreateInfo
			{
				std::shared_ptr<vk::VulkanCore> vkCore;
				std::shared_ptr<vk::CommandPool> commandPool;
				float width;
				float height;
				std::shared_ptr<Scene> scene;
				mat4 viewMat = atta::lookAt(vec3(0,10,0), vec3(0,0,0), vec3(0,0,-1));
				mat4 orthoMat = atta::orthographic(10.0f, 1200.0/900, 10.01);
			};

			Renderer2D(CreateInfo info);
			~Renderer2D();

			void render(VkCommandBuffer commandBuffer);
			void updateCameraMatrix(mat4 viewMatrix);

		private:
			std::shared_ptr<Scene> _scene;
			std::shared_ptr<UniformBuffer2D> _uniformBuffer;
			std::shared_ptr<vk::RenderPass> _renderPass;
			std::shared_ptr<vk::FrameBuffer> _frameBuffer;

			// Pipelines
			std::unique_ptr<GraphicsPipeline2D> _graphicsPipeline;
	};
}

#endif// ATTA_GRAPHICS_2D_RENDERER_H
