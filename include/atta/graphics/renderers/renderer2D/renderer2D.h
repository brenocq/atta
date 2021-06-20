//--------------------------------------------------
// Atta Graphics 2D Renderer
// renderer2D.h
// Date: 2021-02-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_2D_RENDERER_H
#define ATTA_GRAPHICS_2D_RENDERER_H

#include <atta/graphics/renderers/renderer.h>
#include <atta/graphics/renderers/renderer2D/pipelines/graphicsPipeline2D.h>
#include <atta/graphics/renderers/renderer2D/pipelines/linePipeline.h>
#include <atta/graphics/renderers/renderer2D/uniformBuffer2D.h>
#include <atta/core/scene.h>

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
				float orthoHeight = 10.0f;// How many meters will appear vertically on the screen
				float orthoDepth = 10.01f;// maximum depth to render (in meters)
				std::shared_ptr<Scene> scene;
				mat4 viewMat = atta::lookAt(vec3(0,10,0), vec3(0,0,0), vec3(0,0,-1));
				//mat4 orthoMat = atta::orthographic(10.0f, 1200.0/900, 10.01);
			};

			Renderer2D(CreateInfo info);
			~Renderer2D();

			void render(VkCommandBuffer commandBuffer);
			void updateCameraMatrix(mat4 viewMatrix);
			void resize(unsigned width, unsigned height);

		private:
			void createRenderPass();
			void createFrameBuffers();
			void createPipelines();

			std::shared_ptr<Scene> _scene;
			std::shared_ptr<UniformBuffer2D> _uniformBuffer;
			std::shared_ptr<vk::RenderPass> _renderPass;
			std::shared_ptr<vk::FrameBuffer> _frameBuffer;

			// Pipelines
			std::unique_ptr<GraphicsPipeline2D> _graphicsPipeline;
			std::unique_ptr<LinePipeline> _linePipeline;

			// Support
			bool _linePipelineSupport;

			// Orthographic projection matrix info
			float _orthoHeight;
			float _orthoDepth;
	};
}

#endif// ATTA_GRAPHICS_2D_RENDERER_H
