//--------------------------------------------------
// Atta Pre Compiled Header
// atta.cpp
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/atta.h>
#include <atta/eventSystem/events/windowMouseMoveEvent.h>

namespace atta
{
	Atta::Atta():
		_shouldFinish(false)
	{
		_eventDispatcher = std::make_shared<EventDispatcher>();
		_eventDispatcher->subscribe(SID("Window_Close"), BIND_EVENT_FUNC(Atta::onWindowClose));

		_graphicsManager = std::make_unique<GraphicsManager>(_eventDispatcher);
	}

	Atta::~Atta()
	{
		LOG_VERBOSE("Atta", "Finished");
	}

	void Atta::run()
	{
		while(!_shouldFinish)
		{
			_graphicsManager->update();

			_graphicsManager->renderWindow();
		}
	}

	void Atta::onWindowClose(Event& event)
	{
		_shouldFinish = true;
	}
}
