//--------------------------------------------------
// Atta Graphics System
// rendererAPI.h
// Date: 2021-08-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_API_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_API_H

namespace atta
{
	class RendererAPI
	{
	public:
		virtual ~RendererAPI = default;

		enum class API
		{
			NONE, OPENGL
		};

		virtual void init();
		virtual void shutdown();
		virtual void resize(uint32_t width, uint32_t height);

	private:
		
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_API_H
