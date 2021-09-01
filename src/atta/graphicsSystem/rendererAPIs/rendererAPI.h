//--------------------------------------------------
// Atta Graphics System
// rendererAPI.h
// Date: 2021-08-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_RENDERER_API_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_RENDERER_API_H

namespace atta
{
	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;

		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_RENDERER_API_H
