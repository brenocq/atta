//--------------------------------------------------
// Atta Graphics System
// graphicsManager.h
// Date: 2021-08-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_GRAPHICS_MANAGER_H
#define ATTA_GRAPHICS_SYSTEM_GRAPHICS_MANAGER_H
#include <atta/eventSystem/eventDispatcher.h>
#include <atta/core/window.h>

namespace atta
{
	class GraphicsManager final
	{
	public:
		GraphicsManager(std::shared_ptr<EventDispatcher> eventDispatcher);
		~GraphicsManager();

		void update();
		void renderWindow();
		void renderCamera() = delete;

	private:
		std::shared_ptr<EventDispatcher> _eventDispatcher;
		std::unique_ptr<Window> _window;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_GRAPHICS_MANAGER_H
