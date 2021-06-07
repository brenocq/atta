//--------------------------------------------------
// Atta Graphics
// rastRenderer.h
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERERS_RAST_RENDERER_RAST_RENDERER_H
#define ATTA_GRAPHICS_RENDERERS_RAST_RASTERIZATION_RENDERER_H

#include <atta/graphics/renderers/renderer.h>
#include <atta/graphics/renderers/rastRenderer/pipelines/graphicsPipeline.h>
#include <atta/graphics/renderers/rastRenderer/pipelines/pointPipeline.h>
#include <atta/graphics/renderers/rastRenderer/pipelines/linePipeline.h>
#include <atta/graphics/renderers/rastRenderer/pipelines/maskPipeline.h>
#include <atta/graphics/renderers/rastRenderer/pipelines/outlinePipeline.h>
#include <atta/graphics/renderers/rastRenderer/pipelines/skyboxPipeline.h>
#include <atta/core/scene.h>
#include <atta/objects/object.h>

namespace atta
{
	class RastRenderer : public Renderer
	{
		public:
			struct CreateInfo
			{
				std::shared_ptr<vk::VulkanCore> vkCore;
				std::shared_ptr<vk::CommandPool> commandPool;
				float width;
				float height;
				std::shared_ptr<Scene> scene;
				mat4 viewMat = atta::lookAt(vec3(-10,-1,0), vec3(0,0,0), vec3(0,1,0));
				mat4 projMat = atta::perspective(atta::radians(45.0), 1200.0/900, 0.01f, 1000.0f);
			};

			RastRenderer(CreateInfo info);
			~RastRenderer();

			void render(VkCommandBuffer commandBuffer);
			void updateCameraMatrix(mat4 viewMatrix);
			void resize(unsigned width, unsigned height);

		private:
			void createRenderPass();
			void createFrameBuffers();
			void createPipelines();

			std::shared_ptr<Scene> _scene;
			std::shared_ptr<vk::UniformBuffer> _uniformBuffer;
			std::shared_ptr<vk::RenderPass> _renderPass;
			std::shared_ptr<vk::FrameBuffer> _frameBuffer;

			// Pipelines
			std::unique_ptr<vk::GraphicsPipeline> _graphicsPipeline;
			std::unique_ptr<vk::PointPipeline> _pointPipeline;
			std::unique_ptr<vk::LinePipeline> _linePipeline;
			std::unique_ptr<vk::MaskPipeline> _maskPipeline;
			std::unique_ptr<vk::OutlinePipeline> _outlinePipeline;
			std::unique_ptr<vk::SkyboxPipeline> _skyboxPipeline;

			// Support
			bool _linePipelineSupport;
	};
}

#endif// ATTA_GRAPHICS_RENDERERS_RAST_RASTERIZATION_RENDERER_H
