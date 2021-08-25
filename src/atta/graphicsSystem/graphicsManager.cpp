//--------------------------------------------------
// Atta Graphics System
// graphicsManager.cpp
// Date: 2021-08-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/graphicsManager.h>

namespace atta
{
	GraphicsManager::GraphicsManager(std::shared_ptr<EventManager> eventManager):
		_eventManager(eventManager)
	{
		Window::CreateInfo windowInfo = {};
		windowInfo.eventManager = _eventManager;
		_window = std::make_unique<Window>(windowInfo);
	}

	GraphicsManager::~GraphicsManager()
	{

	}

	void GraphicsManager::update()
	{
		_window->update();
	}

	void GraphicsManager::renderWindow()
	{

	}
}
