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
		_window = std::make_unique<Window>((Window::CreateInfo){.eventDispatcher = _eventDispatcher });

		_eventDispatcher->subscribe(SID("Window_MouseMove"), BIND_EVENT_FUNC(Atta::onEvent));
		_eventDispatcher->subscribe(SID("Window_Resize"), BIND_EVENT_FUNC(Atta::onEvent));
		_eventDispatcher->subscribe(SID("Window_Focus"), BIND_EVENT_FUNC(Atta::onEvent));
		_eventDispatcher->subscribe(SID("Window_Close"), BIND_EVENT_FUNC(Atta::onEvent));
		_eventDispatcher->subscribe(SID("Window_MouseButton"), BIND_EVENT_FUNC(Atta::onEvent));
		_eventDispatcher->subscribe(SID("Window_KeyboardButton"), BIND_EVENT_FUNC(Atta::onEvent));
		_eventDispatcher->subscribe(SID("Window_MouseScroll"), BIND_EVENT_FUNC(Atta::onEvent));
		_eventDispatcher->subscribe(SID("Window_Close"), BIND_EVENT_FUNC(Atta::onWindowClose));
	}

	Atta::~Atta()
	{
		LOG_VERBOSE("Atta", "Finished");
	}

	void Atta::run()
	{
		while(!_shouldFinish)
			_window->update();
	}

	void Atta::onEvent(Event& event)
	{
		LOG_DEBUG("Atta", "$0", event.toString());
		switch(event.getType())
		{
			case SID("Window_MouseMove"):
			{
				WindowMouseMoveEvent& e = reinterpret_cast<WindowMouseMoveEvent&>(event);
				LOG_DEBUG("Atta", "$0 $1 $2", e.toString(), e.x, e.y);
			}
		}
	}

	void Atta::onWindowClose(Event& event)
	{
		_shouldFinish = true;
	}
}
