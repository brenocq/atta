//--------------------------------------------------
// Atta Pre Compiled Header
// atta.cpp
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/atta.h>

#include <atta/eventSystem/eventManager.h>
#include <atta/eventSystem/events/windowCloseEvent.h>
#include <atta/eventSystem/events/simulationStartEvent.h>
#include <atta/eventSystem/events/simulationPlayEvent.h>
#include <atta/eventSystem/events/simulationPauseEvent.h>
#include <atta/eventSystem/events/simulationStopEvent.h>

#include <atta/memorySystem/memoryManager.h>
#include <atta/componentSystem/componentManager.h>
#include <atta/fileSystem/fileManager.h>
#include <atta/scriptSystem/scriptManager.h>
#include <atta/resourceSystem/resourceManager.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/graphicsSystem/pipeline.h>
#include <atta/sensorSystem/sensorManager.h>
#include <atta/uiSystem/uiManager.h>

// Include execute code
#include <atta/scriptSystem/script.h>
#include <atta/componentSystem/components/scriptComponent.h>

// TODO move to another class
#include <ctime>

namespace atta
{
    Atta::Atta(const CreateInfo& info):
        _shouldFinish(false), _simulationState(SimulationState::NOT_RUNNING)
    {
        FileManager::startUp();

        uint64_t size = 2UL * 1024UL * 1024UL * 1024UL;
        _mainAllocator = new StackAllocator(size);// Allocate 2GB for the whole system
        MemoryManager::registerAllocator(SSID("MainAllocator"), 
                static_cast<Allocator*>(_mainAllocator));

        ResourceManager::startUp();
        ComponentManager::startUp();
        GraphicsManager::startUp();
        ui::UIManager::startUp();
        ScriptManager::startUp();
        SensorManager::startUp();

        // Atta is the last one to reveice events
        EventManager::subscribe<WindowCloseEvent>(BIND_EVENT_FUNC(Atta::onWindowClose));
        EventManager::subscribe<SimulationStartEvent>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));
        EventManager::subscribe<SimulationPlayEvent>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));
        EventManager::subscribe<SimulationPauseEvent>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));
        EventManager::subscribe<SimulationStopEvent>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));

        if(!info.projectFile.empty())
            FileManager::openProject(info.projectFile);
    }

    Atta::~Atta()
    {
        SensorManager::shutDown();
        ScriptManager::shutDown();
        ui::UIManager::shutDown();
        GraphicsManager::shutDown();
        ComponentManager::shutDown();
        ResourceManager::shutDown();
        FileManager::shutDown();

        delete _mainAllocator;
        LOG_VERBOSE("Atta", "Finished");
    }

    void Atta::run()
    {
        float dt = 0.01;
        while(!_shouldFinish)
        {
            // TODO Abstract this
            if(_simulationState == SimulationState::RUNNING)
            {
                // Execute graphics update every X seconds
                static clock_t lastTime = std::clock();
                const clock_t currTime = std::clock();
                float timeDiff = float(currTime-lastTime)/CLOCKS_PER_SEC;
                if(timeDiff > 0.03f)
                {
                    GraphicsManager::update();
                    lastTime = currTime;
                }
            }
            else
                // Update graphics every frame
                GraphicsManager::update();

            ProjectScript* project = ScriptManager::getProjectScript();
            if(_simulationState == SimulationState::RUNNING)
            {
                SensorManager::update(dt);

                if(project)
                    project->onUpdateBefore(dt);

                std::vector<EntityId> entities = ComponentManager::getEntities();
                for(EntityId entity : entities)
                {
                    ScriptComponent* scriptComponent = ComponentManager::getEntityComponent<ScriptComponent>(entity);
                    if(scriptComponent)
                    {
                        Script* script = ScriptManager::getScript(scriptComponent->sid);
                        if(script)
                            script->update(entity, dt);
                    }
                }

                for(auto& factory : ComponentManager::getFactories())
                {
                    ScriptComponent* scriptComponent = factory.getComponent<ScriptComponent>();
                    if(scriptComponent)
                    {
                        Script* script = ScriptManager::getScript(scriptComponent->sid);
                        std::vector<uint8_t*> memories = factory.getMemories();
                        if(script)
                            for(uint64_t i = 0; i < factory.getMaxClones(); i++)
                                script->update(memories, i, dt);
                    }
                }

                if(project)
                    project->onUpdateAfter(dt);
            }
            else
                if(project)
                    project->onUIUpdate();

            FileManager::update();
        }

        FileManager::closeProject();
    }

    void Atta::onWindowClose(Event& event)
    {
        _shouldFinish = true;
    }

    void Atta::onSimulationStateChange(Event& event)
    {
        ProjectScript* project = ScriptManager::getProjectScript();
        switch(event.getType())
        {
            case SimulationStartEvent::type:
                {
                    if(project) project->onStart();
                    _simulationState = SimulationState::RUNNING;
                    break;
                }
            case SimulationPlayEvent::type:
                {
                    if(project) project->onPlay();
                    _simulationState = SimulationState::RUNNING;
                    break;
                }
            case SimulationPauseEvent::type:
                {
                    if(project) project->onPause();
                    _simulationState = SimulationState::PAUSED;
                    break;
                }
            case SimulationStopEvent::type:
                {
                    if(project) project->onStop();
                    _simulationState = SimulationState::NOT_RUNNING;
                    break;
                }
            default:
                {
                    LOG_WARN("Atta", "Unknown simulation event");
                }
        }
    }
}
