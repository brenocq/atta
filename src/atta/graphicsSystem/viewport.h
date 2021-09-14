//--------------------------------------------------
// Atta Graphics System
// viewport.h
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_VIEWPORT_H
#define ATTA_GRAPHICS_SYSTEM_VIEWPORT_H
#include <atta/core/stringId.h>
#include <atta/graphicsSystem/renderers/renderer.h>

namespace atta
{
	class Viewport final
	{
	public:
		struct CreateInfo
		{
			StringId sid = StringId("Unnamed viewport");
			std::shared_ptr<Renderer> renderer = nullptr;
		};
		Viewport(CreateInfo info);
		~Viewport();

		void render();
		void resize(uint32_t width, uint32_t height);

		StringId getSID() { return _sid; }
		uint32_t getWidth() const { return _renderer->getWidth(); }
		uint32_t getHeight() const { return _renderer->getHeight(); }
		void* getImGuiTexture() const { return _renderer->getImGuiTexture(); }

	private:
		StringId _sid;
		std::shared_ptr<Renderer> _renderer;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_VIEWPORT_H
