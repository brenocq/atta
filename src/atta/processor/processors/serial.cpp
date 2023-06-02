//--------------------------------------------------
// Atta Processor Module
// serial.cpp
// Date: 2023-02-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/prototype.h>
#include <atta/component/components/script.h>
#include <atta/component/interface.h>
#include <atta/physics/interface.h>
#include <atta/processor/interface.h>
#include <atta/processor/processors/serial.h>
#include <atta/script/registry/controllerRegistry.h>
#include <atta/script/registry/worldRegistry.h>
#include <atta/sensor/interface.h>

namespace atta::processor {

Serial::Serial() : Processor(Type::SERIAL) { _pause = true; }

Serial::~Serial() {}

// template <typename Script>
// void Serial::run(cmp::Entity entity, float dt, uint32_t num) {
//     cmp::EntityId start = entity.getId();
//     cmp::EntityId end = entity.getId() + num;
//     for (cmp::EntityId i = start; i < end; i++) {
//         Script script;
//         script.entity = cmp::Entity(i);
//         script.dt = dt;
//         script.update();
//     }
// }

void Serial::start() {
    _stop = false;
    _pause = true;
    _thread = std::thread(&Serial::loop, this);
}

void Serial::pause() {
    std::unique_lock<std::mutex> lock(_mtx);
    // Ask to pause
    _pause = true;
    // Wait until paused
    _cvWasPaused.wait(lock, [] { return true; });
}

void Serial::resume() {
    // Ask to resume
    std::lock_guard<std::mutex> lock(_mtx);
    _pause = false;
    _cvPause.notify_one();
}

void Serial::stop() {
    _stop = true;
    if (_pause)
        resume();
    _thread.join();
}

void Serial::loop() {
    script::WorldRegistry::onStart();
    while (!_stop) {
        // Handle pause
        {
            std::unique_lock<std::mutex> lock(_mtx);
            if (_pause) {
                _cvWasPaused.notify_one();                       // Notify it was paused
                _cvPause.wait(lock, [this] { return !_pause; }); // Wait until not paused
            }
            if (_stop)
                break;
        }

        float dt = processor::getDt();
        physics::update(dt);
        sensor::update(dt);
        script::WorldRegistry::onUpdateBefore(dt);

        for (auto& factory : component::getFactories()) {
            component::Script* script = factory.getPrototype().get<component::Script>();
            if (script) {
                const script::ControllerRegistry* controller = script::ControllerRegistry::getRegistry(script->sid);
                component::EntityId start = factory.getFirstClone();
                component::EntityId end = start + factory.getMaxClones();
                for (component::EntityId i = start; i < end; i++)
                    controller->update(component::Entity(i));
            }
        }

        script::WorldRegistry::onUpdateAfter(dt);
    }
    script::WorldRegistry::onStop();
}

} // namespace atta::processor
