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
#include <atta/event/events/simulationStep.h>
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

namespace atta {
Atta::Atta(const CreateInfo& info) : _shouldFinish(false) {
    Config::init();
    file::startUp();

    uint64_t size = 1.0 * 1024UL * 1024UL * 1024UL;
    _mainAllocator = new memory::StackAllocator(size); // Allocate 1.0GB for the whole system
    memory::registerAllocator(SSID("MainAllocator"), static_cast<memory::Allocator*>(_mainAllocator));

    resource::startUp();
    component::startUp();
    graphics::startUp();
    ui::startUp();
    physics::startUp();
    sensor::startUp();
    script::startUp();

    ros_node = std::make_shared<ros>();
    ros_node->publishData("Started");
    // Atta is the last one to reveice events
    event::subscribe<event::WindowClose>(BIND_EVENT_FUNC(Atta::onWindowClose));
    event::subscribe<event::SimulationStart>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));
    event::subscribe<event::SimulationStep>(BIND_EVENT_FUNC(Atta::onSimulationStateChange));
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
    if (!info.projectFile.empty())
        file::openProject(info.projectFile);
#endif

    _currStep = _lastStep = std::clock();
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
    PROFILE();
    _currStep = std::clock();
    const float timeDiff = float(_currStep - _lastStep) / CLOCKS_PER_SEC;
    ros_node->publishData("another loop");
    if (Config::getState() == Config::State::RUNNING) {
        if (Config::getDesiredStepSpeed() == 0.0f) {
            // Step as fast as possible
            step();
        } else {
            // Step with delay to be closer to desired step speed
            if (timeDiff >= Config::getDt() / Config::getDesiredStepSpeed())
                step();
        }
    } else if (Config::getState() == Config::State::PAUSED && _shouldStep) {
        _shouldStep = false;
        step();
    }

    script::ProjectScript* project = script::getProjectScript();
    if (project)
        project->onAttaLoop();

    file::update();
    graphics::update();
    resource::update();
}

void Atta::step() {
    PROFILE();
    const float timeDiff = float(_currStep - _lastStep) / CLOCKS_PER_SEC;
    Config::setRealStepSpeed(Config::getDt() / timeDiff);
    _lastStep = _currStep;

    float dt = Config::getDt(); // Saving dt because project script may change the dt
    physics::update(dt);
    sensor::update(dt);
    script::update(dt);
    //ros::update();
    Config::getInstance()._time += dt;
}

void Atta::onWindowClose(event::Event& event) { _shouldFinish = true; }

void Atta::onSimulationStateChange(event::Event& event) {
    script::ProjectScript* project = script::getProjectScript();
    switch (event.getType()) {
        case event::SimulationStart::type: {
            Config::getInstance()._state = Config::State::RUNNING;
            if (project)
                project->onStart();
            break;
        }
        case event::SimulationContinue::type: {
            Config::getInstance()._state = Config::State::RUNNING;
            if (project)
                project->onContinue();
            break;
        }
        case event::SimulationPause::type: {
            Config::getInstance()._state = Config::State::PAUSED;
            if (project)
                project->onPause();
            break;
        }
        case event::SimulationStop::type: {
            Config::getInstance()._state = Config::State::IDLE;
            Config::getInstance()._time = 0.0f;
            Config::getInstance()._realStepSpeed = 0.0f;
            if (project)
                project->onStop();
            break;
        }
        case event::SimulationStep::type: {
            if (Config::getState() == Config::State::IDLE) {
                event::SimulationStart e;
                event::publish(e);
            }
            if (Config::getState() == Config::State::RUNNING) {
                event::SimulationPause e;
                event::publish(e);
            }
            _shouldStep = true;
            break;
        }
        default: {
            LOG_WARN("Atta", "Unknown simulation event");
        }
    }
}
} // namespace atta
