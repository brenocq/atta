//--------------------------------------------------
// Atta Graphics System
// renderer.h
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_H
#include <atta/core/stringId.h>
#include <atta/graphicsSystem/framebuffer.h>

namespace atta
{
	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual void render() = 0;
		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;
		virtual void* getImGuiTexture() const = 0;

	protected:
		uint32_t _width;
		uint32_t _height;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_H
