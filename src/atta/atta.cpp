//--------------------------------------------------
// Atta
// atta.cpp
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/atta.h>

#include <atta/event/eventManager.h>
#include <atta/event/events/simulationContinueEvent.h>
#include <atta/event/events/simulationPauseEvent.h>
#include <atta/event/events/simulationStartEvent.h>
#include <atta/event/events/simulationStopEvent.h>
#include <atta/event/events/windowCloseEvent.h>

#include <atta/component/manager.h>
#include <atta/file/manager.h>
#include <atta/graphics/manager.h>
#include <atta/memory/manager.h>
#include <atta/physics/manager.h>
#include <atta/resource/manager.h>
#include <atta/script/manager.h>
#include <atta/sensor/manager.h>
#include <atta/ui/manager.h>

#include <atta/cmakeConfig.h>
#include <atta/core/config.h>
#include <atta/graphics/pipeline.h>

// Include execute code
#include <atta/component/components/prototypeComponent.h>
#include <atta/component/components/scriptComponent.h>
#include <atta/script/script.h>

// TODO move to another class
#include <ctime>

namespace atta {
Atta::Atta(const CreateInfo& info) : _shouldFinish(false), _simulationState(SimulationState::NOT_RUNNING) {
    Config::init();
    file::Manager::startUp();

    uint64_t size = 1.0 * 1024UL * 1024UL * 1024UL;
    _mainAllocator = new StackAllocator(size); // Allocate 1.0GB for the whole system
    MemoryManager::registerAllocator(SSID("MainAllocator"), static_cast<Allocator*>(_mainAllocator));

    component::Manager::startUp();
    resource::Manager::startUp();
    graphics::Manager::startUp();
    ui::UIManager::startUp();
    script::Manager::startUp();
    physics::Manager::startUp();
    sensor::Manager::startUp();

    // Atta is the last one to reveice events
    event::Manager::subscribe<WindowCloseEvent>(BIND_EVENT_FUNC(Atta::onWindowClose));
    event::Manager::subscribe<SimulationStartEvent>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));
    event::Manager::subscribe<SimulationContinueEvent>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));
    event::Manager::subscribe<SimulationPauseEvent>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));
    event::Manager::subscribe<SimulationStopEvent>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));

    LOG_SUCCESS("Atta", "Initialized");
#ifdef ATTA_STATIC_PROJECT
    fs::path projectFile = fs::path(ATTA_STATIC_PROJECT_FILE);
    // If project is built statically, open project
    file::Manager::openProject(projectFile);

    if (!info.projectFile.empty() && info.projectFile != projectFile)
        LOG_WARN("Atta", "Project [w]$0[] will not be open because atta was built statically linked to [w]$1[]", info.projectFile, projectFile);
#else
    // If a project was defined as argument, open project
    if (!info.projectFile.empty())
        file::Manager::openProject(info.projectFile);
#endif
}

Atta::~Atta() {
    // TODO ask user if should close or not
    // file::Manager::saveProject();
    file::Manager::closeProject();
    sensor::Manager::shutDown();
    physics::Manager::shutDown();
    script::Manager::shutDown();
    ui::Manager::shutDown();
    graphics::Manager::shutDown();
    resource::Manager::shutDown();
    component::Manager::shutDown();
    file::Manager::shutDown();

    delete _mainAllocator;
    LOG_SUCCESS("Atta", "Finished");
}

#ifdef ATTA_OS_WEB
#include <emscripten.h>
void attaLoop(void* atta) { ((Atta*)atta)->loop(); }

void Atta::run() {
    // When atta is being executed by the browser, the browser controls the main loop
    // emscripten_request_animation_frame_loop(attaLoop, this);
    emscripten_set_main_loop_arg((em_arg_callback_func)attaLoop, this, 0, 1);
}
#else
void Atta::run() {
    while (!_shouldFinish)
        loop();
}
#endif

void Atta::loop() {
    if (_shouldFinish)
        return;
    float dt = Config::getDt();

    ProjectScript* project = script::Manager::getProjectScript();
    if (_simulationState == SimulationState::RUNNING) {
        physics::Manager::update(dt);
        sensor::Manager::update(dt);

        if (project)
            project->onUpdateBefore(dt);

        // Run entity scripts
        // TODO keep list of entities that have script (and are not prototype entities)
        std::vector<EntityId> entities = component::Manager::getScriptView();
        for (EntityId entity : entities) {
            ScriptComponent* scriptComponent = component::Manager::getEntityComponent<ScriptComponent>(entity);
            PrototypeComponent* prototypeComponent = component::Manager::getEntityComponent<PrototypeComponent>(entity);
            if (scriptComponent && !prototypeComponent) {
                // std::vector<Component*> components = component::Manager::getEntityComponents(entity);
                Script* script = script::Manager::getScript(scriptComponent->sid);
                if (script)
                    script->update(Entity(entity), dt);
            }
        }

        // Run clone scripts
        for (auto& factory : component::Manager::getFactories())
            factory.runScripts(dt);

        if (project)
            project->onUpdateAfter(dt);
    }
    if (project)
        project->onAttaLoop();

    sensor::Manager::update();
    file::Manager::update();

    if (_simulationState == SimulationState::RUNNING) {
        // Execute graphics update every X seconds
        // TODO let the user control how much time is spent rendering
        static clock_t lastTime = std::clock();
        const clock_t currTime = std::clock();
        float timeDiff = float(currTime - lastTime) / CLOCKS_PER_SEC;
        if (timeDiff > 0.03f) {
            graphics::Manager::update();
            lastTime = currTime;
        }
    } else {
        // Update graphics every frame
        graphics::Manager::update();
    }
}

void Atta::onWindowClose(Event& event) { _shouldFinish = true; }

void Atta::onSimulationStateChange(Event& event) {
    ProjectScript* project = script::Manager::getProjectScript();
    switch (event.getType()) {
    case SimulationStartEvent::type: {
        if (project)
            project->onStart();
        _simulationState = SimulationState::RUNNING;
        break;
    }
    case SimulationContinueEvent::type: {
        if (project)
            project->onContinue();
        _simulationState = SimulationState::RUNNING;
        break;
    }
    case SimulationPauseEvent::type: {
        if (project)
            project->onPause();
        _simulationState = SimulationState::PAUSED;
        break;
    }
    case SimulationStopEvent::type: {
        if (project)
            project->onStop();
        _simulationState = SimulationState::NOT_RUNNING;
        break;
    }
    default: {
        LOG_WARN("Atta", "Unknown simulation event");
    }
    }
}
} // namespace atta
