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

Serial::Serial() : Processor(Type::SERIAL) { }

Serial::~Serial() {}

void Serial::startThread() { _thread = std::thread(&Serial::loop, this); }

void Serial::loop() {
    script::WorldRegistry::onStart();
    float dt = processor::getDt();
    while (shouldRun()) {
        physics::update(dt);
        sensor::update(dt);
        script::WorldRegistry::onUpdateBefore();

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

        script::WorldRegistry::onUpdateAfter();
    }
    script::WorldRegistry::onStop();
}

} // namespace atta::processor
