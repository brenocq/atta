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
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLRenderer.h>
#include <atta/graphicsSystem/layers/layerStack.h>

namespace atta
{
	class GraphicsManager final
	{
	public:
		GraphicsManager(std::shared_ptr<EventManager> eventManager);
		~GraphicsManager();

		void update();

	private:
		std::shared_ptr<EventManager> _eventManager;
		std::shared_ptr<Window> _window;
		std::shared_ptr<OpenGLRenderer> _rendererAPI;

		// Layer stack
		std::unique_ptr<LayerStack> _layerStack;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_GRAPHICS_MANAGER_H
