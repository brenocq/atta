//--------------------------------------------------
// Atta Pre Compiled Header
// atta.cpp
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/atta.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/eventSystem/events/windowMouseMoveEvent.h>
#include <atta/memorySystem/memoryManager.h>

namespace atta
{
	Atta::Atta():
		_shouldFinish(false)
	{
		_mainAllocator = new StackAllocator(1*1024*1024*1024L);// Allocate 1GB for the whole system
		MemoryManager::registerAllocator(SID("Main"), static_cast<Allocator*>(_mainAllocator));

		EventManager::subscribe(SID("Window_Close"), BIND_EVENT_FUNC(Atta::onWindowClose));

		_graphicsManager = new GraphicsManager();
	}

	Atta::~Atta()
	{
		delete _graphicsManager;
		delete _mainAllocator;
		LOG_VERBOSE("Atta", "Finished");
	}

	void Atta::run()
	{
		while(!_shouldFinish)
		{
			_graphicsManager->update();
		}
	}

	void Atta::onWindowClose(Event& event)
	{
		_shouldFinish = true;
	}
}
