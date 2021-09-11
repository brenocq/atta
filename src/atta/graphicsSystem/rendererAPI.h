//--------------------------------------------------
// Atta Graphics System
// rendererAPI.h
// Date: 2021-08-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_API_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_API_H
#include <atta/core/stringId.h>

namespace atta
{
	class RendererAPI
	{
	public:
		enum Type
		{
			OPENGL = 0,
			VULKAN
		};

		RendererAPI(Type type);
		virtual ~RendererAPI() = default;

		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;

		virtual void renderMesh(StringId meshSid) = 0;
	
		Type getType() const { return _type; }

	protected:
		Type _type;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_API_H
