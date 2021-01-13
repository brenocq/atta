//--------------------------------------------------
// Atta Graphics
// rastRenderer.h
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RASTERIZATION_RENDERER_H
#define ATTA_GRAPHICS_RASTERIZATION_RENDERER_H

#include "simulator/graphics/renderers/renderer.h"
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
			};

			RastRenderer(CreateInfo info);
			~RastRenderer();

			void render();

		private:
			std::shared_ptr<Scene> _scene;
	};
}

#endif// ATTA_GRAPHICS_RASTERIZATION_RENDERER_H
