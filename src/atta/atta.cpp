//--------------------------------------------------
// Atta Pre Compiled Header
// atta.cpp
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/atta.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/eventSystem/events/windowCloseEvent.h>
#include <atta/memorySystem/memoryManager.h>
#include <atta/componentSystem/componentManager.h>
#include <atta/fileSystem/fileManager.h>
#include <atta/scriptSystem/scriptManager.h>
#include <atta/resourceSystem/resourceManager.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/graphicsSystem/pipeline.h>

// Include execute code
#include <atta/scriptSystem/script.h>
#include <atta/componentSystem/components/scriptComponent.h>

namespace atta
{
	Atta::Atta(const CreateInfo& info):
		_shouldFinish(false)
	{
		FileManager::startUp();

		uint64_t size = 2UL * 1024UL * 1024UL * 1024UL;
		_mainAllocator = new StackAllocator(size);// Allocate 2GB for the whole system
		MemoryManager::registerAllocator(SSID("MainAllocator"), 
				static_cast<Allocator*>(_mainAllocator));

		ResourceManager::startUp();
		ComponentManager::startUp();

		EventManager::subscribe<WindowCloseEvent>(BIND_EVENT_FUNC(Atta::onWindowClose));

		GraphicsManager::startUp();
		ScriptManager::startUp();

		if(info.projectFile != "")
			FileManager::openProject(info.projectFile);
	}

	Atta::~Atta()
	{
		ScriptManager::shutDown();
		GraphicsManager::shutDown();
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
			GraphicsManager::update();

			ProjectScript* project = ScriptManager::getProjectScript();

			if(project)
				project->onUpdateBefore(0.01);

			std::vector<EntityId> entities = ComponentManager::getEntities();
			for(EntityId entity : entities)
			{
				ScriptComponent* scriptComponent = ComponentManager::getEntityComponent<ScriptComponent>(entity);
				if(scriptComponent != nullptr)
				{
					Script* script = ScriptManager::getScript(scriptComponent->sid);
					if(script != nullptr)
						script->update(entity, 0.01);
				}
			}

			if(project)
				project->onUpdateAfter(0.01);

			FileManager::update();
		}

		FileManager::closeProject();
	}

	void Atta::onWindowClose(Event& event)
	{
		_shouldFinish = true;
	}
}
