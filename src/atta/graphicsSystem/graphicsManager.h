//--------------------------------------------------
// Atta Graphics System
// graphicsManager.h
// Date: 2021-08-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_GRAPHICS_MANAGER_H
#define ATTA_GRAPHICS_SYSTEM_GRAPHICS_MANAGER_H
#include <atta/eventSystem/eventManager.h>
#include <atta/graphicsSystem/window.h>

namespace atta
{
	class GraphicsManager final
	{
	public:
		GraphicsManager(std::shared_ptr<EventManager> eventManager);
		~GraphicsManager();

		void update();
		void renderWindow();
		void renderCamera() = delete;

	private:
		std::shared_ptr<EventManager> _eventManager;
		std::unique_ptr<Window> _window;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_GRAPHICS_MANAGER_H
