//--------------------------------------------------
// Atta Graphics
// rastRenderer.h
// Date: 2021-01-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RASTERIZATION_RENDERER_H
#define ATTA_GRAPHICS_RASTERIZATION_RENDERER_H

#include "renderer.h"

namespace atta
{
	class RastRenderer : public Renderer
	{
		public:
			RastRenderer();
			~RastRenderer();

			void render();

		private:
	};
}

#endif// ATTA_GRAPHICS_RASTERIZATION_RENDERER_H
