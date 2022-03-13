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
#include <atta/eventSystem/events/simulationContinueEvent.h>
#include <atta/eventSystem/events/simulationPauseEvent.h>
#include <atta/eventSystem/events/simulationStopEvent.h>

#include <atta/memorySystem/memoryManager.h>
#include <atta/componentSystem/componentManager.h>
#include <atta/fileSystem/fileManager.h>
#include <atta/scriptSystem/scriptManager.h>
#include <atta/resourceSystem/resourceManager.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/graphicsSystem/pipeline.h>
#include <atta/physicsSystem/physicsManager.h>
#include <atta/sensorSystem/sensorManager.h>
#include <atta/uiSystem/uiManager.h>
#include <atta/core/config.h>

// Include execute code
#include <atta/scriptSystem/script.h>
#include <atta/componentSystem/components/scriptComponent.h>
#include <atta/componentSystem/components/prototypeComponent.h>

// TODO move to another class
#include <ctime>

namespace atta
{
    Atta::Atta(const CreateInfo& info):
        _shouldFinish(false), _simulationState(SimulationState::NOT_RUNNING)
    {
        Config::init();
        FileManager::startUp();

        uint64_t size = 1.5 * 1024UL * 1024UL * 1024UL;
        _mainAllocator = new StackAllocator(size);// Allocate 1.5GB for the whole system
        MemoryManager::registerAllocator(SSID("MainAllocator"), 
                static_cast<Allocator*>(_mainAllocator));

        ComponentManager::startUp();
        ResourceManager::startUp();
        GraphicsManager::startUp();
        ui::UIManager::startUp();
        ScriptManager::startUp();
        PhysicsManager::startUp();
        SensorManager::startUp();

        // Atta is the last one to reveice events
        EventManager::subscribe<WindowCloseEvent>(BIND_EVENT_FUNC(Atta::onWindowClose));
        EventManager::subscribe<SimulationStartEvent>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));
        EventManager::subscribe<SimulationContinueEvent>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));
        EventManager::subscribe<SimulationPauseEvent>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));
        EventManager::subscribe<SimulationStopEvent>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));

        LOG_SUCCESS("Atta", "Initialized");
        if(!info.projectFile.empty())
            FileManager::openProject(info.projectFile);
    }

    Atta::~Atta()
    {
		// TODO ask user if should close or not
        //FileManager::saveProject();
        FileManager::closeProject();
        SensorManager::shutDown();
        PhysicsManager::shutDown();
        ScriptManager::shutDown();
        ui::UIManager::shutDown();
        GraphicsManager::shutDown();
        ResourceManager::shutDown();
        ComponentManager::shutDown();
        FileManager::shutDown();

        delete _mainAllocator;
        LOG_SUCCESS("Atta", "Finished");
    }

#ifdef ATTA_OS_WEB
    #include <emscripten.h>
    void attaLoop(void* atta)
    {
        ((Atta*)atta)->loop();
    }

    void Atta::run()
    {
        // When atta is being executed by the browser, the browser controls the main loop
        //emscripten_request_animation_frame_loop(attaLoop, this);
        emscripten_set_main_loop_arg((em_arg_callback_func)attaLoop, this, 0, 1);
    }
#else
    void Atta::run()
    {
        while(!_shouldFinish)
            loop();
    }
#endif

    void Atta::loop()
    {
        if(_shouldFinish) return;
        float dt = Config::getDt();

        ProjectScript* project = ScriptManager::getProjectScript();
        if(_simulationState == SimulationState::RUNNING)
        {
			PhysicsManager::update(dt);
            SensorManager::update(dt);

            if(project)
                project->onUpdateBefore(dt);

            // Run entity scripts
            // TODO keep list of entities that have script (and are not prototype entities)
            std::vector<EntityId> entities = ComponentManager::getScriptView();
            for(EntityId entity : entities)
            {
                ScriptComponent* scriptComponent = ComponentManager::getEntityComponent<ScriptComponent>(entity);
                PrototypeComponent* prototypeComponent = ComponentManager::getEntityComponent<PrototypeComponent>(entity);
                if(scriptComponent && !prototypeComponent)
                {
                    //std::vector<Component*> components = ComponentManager::getEntityComponents(entity);
                    Script* script = ScriptManager::getScript(scriptComponent->sid);
                    if(script)
                        script->update(Entity(entity), dt);
                }
            }

            // Run clone scripts
            for(auto& factory : ComponentManager::getFactories())
                factory.runScripts(dt);

            if(project)
                project->onUpdateAfter(dt);
        }
        if(project)
            project->onAttaLoop();

        SensorManager::update();
        FileManager::update();

        if(_simulationState == SimulationState::RUNNING)
        {
            // Execute graphics update every X seconds
			// TODO let the user control how much time is spent rendering
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
        {
            // Update graphics every frame
            GraphicsManager::update();
        }
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
            case SimulationContinueEvent::type:
                {
                    if(project) project->onContinue();
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
