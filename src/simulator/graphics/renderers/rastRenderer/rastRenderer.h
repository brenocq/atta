//--------------------------------------------------
// Atta Graphics
// rastRenderer.h
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RASTERIZATION_RENDERER_H
#define ATTA_GRAPHICS_RASTERIZATION_RENDERER_H

#include "simulator/graphics/renderers/renderer.h"
#include "simulator/graphics/renderers/rastRenderer/pipelines/graphicsPipeline.h"
#include "simulator/graphics/renderers/rastRenderer/pipelines/linePipeline.h"
#include "simulator/graphics/renderers/rastRenderer/pipelines/maskPipeline.h"
#include "simulator/graphics/renderers/rastRenderer/pipelines/outlinePipeline.h"
#include "simulator/graphics/renderers/rastRenderer/pipelines/skyboxPipeline.h"
#include "simulator/core/scene.h"

namespace atta
{
	class RastRenderer : public Renderer
	{
		public:
			struct CreateInfo
			{
				std::shared_ptr<vk::VulkanCore> vkCore;
				float width;
				float height;
				std::shared_ptr<Scene> scene;
				mat4 viewMat;
				mat4 projMat;
			};

			RastRenderer(CreateInfo info);
			~RastRenderer();

			void render(VkCommandBuffer commandBuffer);
			void updateCameraMatrix(mat4 viewMatrix);

		private:
			std::shared_ptr<Scene> _scene;
			std::shared_ptr<vk::UniformBuffer> _uniformBuffer;
			std::shared_ptr<vk::RenderPass> _renderPass;
			std::shared_ptr<vk::FrameBuffer> _frameBuffer;

			// Pipelines
			std::unique_ptr<vk::GraphicsPipeline> _graphicsPipeline;
			std::unique_ptr<vk::LinePipeline> _linePipeline;
			std::unique_ptr<vk::MaskPipeline> _maskPipeline;
			std::unique_ptr<vk::OutlinePipeline> _outlinePipeline;
			std::unique_ptr<vk::SkyboxPipeline> _skyboxPipeline;
	};
}

#endif// ATTA_GRAPHICS_RASTERIZATION_RENDERER_H
