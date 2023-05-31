//--------------------------------------------------
// Atta
// atta.cpp
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/atta.h>

#include <atta/component/interface.h>
#include <atta/file/interface.h>
#include <atta/graphics/interface.h>
#include <atta/memory/interface.h>
#include <atta/physics/interface.h>
#include <atta/processor/interface.h>
#include <atta/resource/interface.h>
#include <atta/script/interface.h>
#include <atta/sensor/interface.h>
#include <atta/ui/interface.h>

#include <atta/event/event.h>
#include <atta/event/events/windowClose.h>

#include <atta/cmakeConfig.h>
#include <atta/utils/config.h>

namespace atta {
Atta::Atta(const CreateInfo& info) : _shouldFinish(false) {
    Config::init();
    file::startUp();

    uint64_t size = 1.0 * 1024UL * 1024UL * 1024UL;
    _mainAllocator = new memory::StackAllocator(size); // Allocate 1.0GB for the whole system
    memory::registerAllocator("MainAllocator", static_cast<memory::Allocator*>(_mainAllocator));

    resource::startUp();
    component::startUp();
    graphics::startUp();
    ui::startUp();
    processor::startUp();
    physics::startUp();
    sensor::startUp();
    script::startUp();

    // Atta is the last one to reveice events
    event::subscribe<event::WindowClose>(BIND_EVENT_FUNC(Atta::onWindowClose));

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

    _currStep = _lastStep = std::chrono::high_resolution_clock::now();
}

Atta::~Atta() {
    // TODO ask user if should close or not
    // file::saveProject();
    file::closeProject();
    script::shutDown();
    sensor::shutDown();
    physics::shutDown();
    processor::shutDown();
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
    // _currStep = std::chrono::high_resolution_clock::now();
    // if (Config::getState() == Config::State::RUNNING) {
    //     if (Config::getDesiredStepSpeed() == 0.0f) {
    //         // Step as fast as possible
    //         step();
    //     } else {
    //         const float timeDiff = std::chrono::duration<float, std::milli>(_currStep - _lastStep).count() / 1000.0;
    //         // Step with delay to be closer to desired step speed
    //         if (timeDiff >= Config::getDt() / Config::getDesiredStepSpeed())
    //             step();
    //     }
    // } else if (Config::getState() == Config::State::PAUSED && _shouldStep) {
    //     _shouldStep = false;
    //     step();
    // }
    // script::WorldRegistry::onAttaLoop();

    // XXX simulation running in another thread

    file::update();
    if (graphics::shouldUpdate()) {
        processor::pause();
        processor::readData();
        graphics::update();
        processor::writeData();
        processor::resume();
    }
}

// void Atta::step() {
//     // PROFILE();
//
//     // float dt = Config::getDt(); // Saving dt because project script may change the dt
//     // physics::update(dt);
//     // sensor::update(dt);
//     // script::update(dt);
//
//     // Update time
//     // const float timeDiff = std::chrono::duration<float, std::milli>(_currStep - _lastStep).count() / 1000.0;
//     // Config::setRealStepSpeed(Config::getRealStepSpeed() * 0.5 + (Config::getDt() / timeDiff) * 0.5);
//     // _lastStep = _currStep;
//     // Config::getInstance()._time += dt;
// }

void Atta::onWindowClose(event::Event& event) { _shouldFinish = true; }

} // namespace atta
