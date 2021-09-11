//--------------------------------------------------
// Atta Graphics System
// fastRenderer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERERS_FAST_RENDERER_H
#define ATTA_GRAPHICS_SYSTEM_RENDERERS_FAST_RENDERER_H
#include <atta/graphicsSystem/pipeline.h>

namespace atta
{
	class FastRenderer
	{
	public:
		FastRenderer();
		~FastRenderer();

		void render();

	private:
		std::shared_ptr<Pipeline> _geometryPipeline;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERERS_FAST_RENDERER_H
