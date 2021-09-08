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
#include <atta/componentSystem/componentManager.h>
#include <atta/fileSystem/fileManager.h>
#include <atta/scriptSystem/scriptManager.h>
#include <atta/resourceSystem/resourceManager.h>

namespace atta
{
	Atta::Atta(CreateInfo info):
		_shouldFinish(false)
	{
		FileManager::startUp();

		uint64_t size = 2UL * 1024UL * 1024UL * 1024UL;
		_mainAllocator = new StackAllocator(size);// Allocate 2GB for the whole system
		MemoryManager::registerAllocator(SSID("MainAllocator"), 
				static_cast<Allocator*>(_mainAllocator));

		ResourceManager::startUp();
		ComponentManager::startUp();

		EventManager::subscribe(SSID("Window_Close"), BIND_EVENT_FUNC(Atta::onWindowClose));

		_graphicsManager = new GraphicsManager();

		ScriptManager::startUp();

		if(info.projectFile != "")
			FileManager::setProjectFile(info.projectFile);
	}

	Atta::~Atta()
	{
		delete _graphicsManager;

		ScriptManager::shutDown();
		ComponentManager::shutDown();
		ResourceManager::shutDown();
		FileManager::shutDown();

		delete _mainAllocator;
		LOG_VERBOSE("Atta", "Finished");
	}

	void Atta::run()
	{
		while(!_shouldFinish)
		{
			_graphicsManager->update();
			FileManager::update();
		}
	}

	void Atta::onWindowClose(Event& event)
	{
		_shouldFinish = true;
	}
}
