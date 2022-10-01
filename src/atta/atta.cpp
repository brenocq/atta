//--------------------------------------------------
// Atta
// atta.cpp
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/atta.h>

#include <atta/event/event.h>
#include <atta/event/events/simulationContinue.h>
#include <atta/event/events/simulationPause.h>
#include <atta/event/events/simulationStart.h>
#include <atta/event/events/simulationStop.h>
#include <atta/event/events/windowClose.h>

#include <atta/component/interface.h>
#include <atta/file/interface.h>
#include <atta/graphics/interface.h>
#include <atta/memory/interface.h>
#include <atta/physics/interface.h>
#include <atta/resource/interface.h>
#include <atta/script/interface.h>
#include <atta/sensor/interface.h>
#include <atta/ui/interface.h>

#include <atta/cmakeConfig.h>
#include <atta/graphics/pipeline.h>
#include <atta/utils/config.h>

// Include execute code
#include <atta/component/components/prototype.h>
#include <atta/component/components/script.h>
#include <atta/script/script.h>

// TODO move to another class
#include <ctime>

namespace atta {
Atta::Atta(const CreateInfo& info) : _shouldFinish(false), _simulationState(SimulationState::NOT_RUNNING) {
    Config::init();
    file::startUp();

    uint64_t size = 1.0 * 1024UL * 1024UL * 1024UL;
    _mainAllocator = new memory::StackAllocator(size); // Allocate 1.0GB for the whole system
    memory::registerAllocator(SSID("MainAllocator"), static_cast<memory::Allocator*>(_mainAllocator));

    resource::startUp();
    component::startUp();
    graphics::startUp();
    ui::startUp();
    script::startUp();
    physics::startUp();
    sensor::startUp();

    // Atta is the last one to reveice events
    event::subscribe<event::WindowClose>(BIND_EVENT_FUNC(Atta::onWindowClose));
    event::subscribe<event::SimulationStart>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));
    event::subscribe<event::SimulationContinue>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));
    event::subscribe<event::SimulationPause>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));
    event::subscribe<event::SimulationStop>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));

    LOG_SUCCESS("Atta", "Initialized");
#ifdef ATTA_STATIC_PROJECT
    fs::path projectFile = fs::path(ATTA_STATIC_PROJECT_FILE);
    // If project is built statically, open project
    file::openProject(projectFile);

    if (!info.projectFile.empty() && info.projectFile != projectFile)
        LOG_WARN("Atta", "Project [w]$0[] will not be open because atta was built statically linked to [w]$1[]", info.projectFile, projectFile);
#else
    // If a project was defined as argument, open project
    if (!info.projectFile.empty()) {
        graphics::update(); // Need to update to register the viewports
        file::openProject(info.projectFile);
    }
#endif
}

Atta::~Atta() {
    // TODO ask user if should close or not
    // file::saveProject();
    file::closeProject();
    sensor::shutDown();
    physics::shutDown();
    script::shutDown();
    ui::shutDown();
    graphics::shutDown();
    component::shutDown();
    resource::shutDown();
    file::shutDown();

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

    script::ProjectScript* project = script::getProjectScript();
    if (_simulationState == SimulationState::RUNNING) {
        physics::update(dt);
        sensor::update(dt);

        if (project)
            project->onUpdateBefore(dt);

        // Run clone scripts
        for (auto& factory : component::getFactories())
            factory.runScripts(dt);

        // Run scripts of other entities (not clones)
        const auto& factories = component::getFactories();
        std::vector<std::pair<component::EntityId, component::EntityId>> beginEndClones(factories.size());
        for (const auto& factory : factories)
            beginEndClones.push_back({factory.getFirstClone(), factory.getLastClone()});
        std::vector<component::EntityId> entities = component::getScriptView();
        for (component::EntityId entity : entities) {
            // Check if it it not clone entity
            for(auto [begin, end] : beginEndClones)
                if(entity >= begin && entity <= end)
                    continue;
            
            // Check if it is not prototype entity
            component::Script* scriptComponent = component::getComponent<component::Script>(entity);
            component::Prototype* prototypeComponent = component::getComponent<component::Prototype>(entity);
            if (scriptComponent && !prototypeComponent) {
                script::Script* script = script::getScript(scriptComponent->sid);
                if (script)
                    script->update(component::Entity(entity), dt);
            }
        }

        if (project)
            project->onUpdateAfter(dt);
    }
    if (project)
        project->onAttaLoop();

    sensor::update();
    file::update();

    if (_simulationState == SimulationState::RUNNING) {
        // Execute graphics with 30FPS
        // TODO let the user control graphics FPS
        static clock_t lastTime = std::clock();
        const clock_t currTime = std::clock();
        float timeDiff = float(currTime - lastTime) / CLOCKS_PER_SEC;
        if (timeDiff > 0.03f) {
            graphics::update();
            lastTime = currTime;
        }
    } else {
        // Update graphics every frame
        graphics::update();
    }
}

void Atta::onWindowClose(event::Event& event) { _shouldFinish = true; }

void Atta::onSimulationStateChange(event::Event& event) {
    script::ProjectScript* project = script::getProjectScript();
    switch (event.getType()) {
        case event::SimulationStart::type: {
            if (project)
                project->onStart();
            _simulationState = SimulationState::RUNNING;
            break;
        }
        case event::SimulationContinue::type: {
            if (project)
                project->onContinue();
            _simulationState = SimulationState::RUNNING;
            break;
        }
        case event::SimulationPause::type: {
            if (project)
                project->onPause();
            _simulationState = SimulationState::PAUSED;
            break;
        }
        case event::SimulationStop::type: {
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
