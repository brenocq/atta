//--------------------------------------------------
// Atta Processor Module
// cpuSerial.cpp
// Date: 2023-02-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/prototype.h>
#include <atta/component/components/script.h>
#include <atta/component/interface.h>
#include <atta/physics/interface.h>
#include <atta/processor/interface.h>
#include <atta/processor/processors/cpuSerial.h>
#include <atta/script/registry/controllerRegistry.h>
#include <atta/script/registry/worldRegistry.h>
#include <atta/sensor/interface.h>
#include <chrono>

#include "/home/breno/Github/brenocq-atta/ants/src/ant.h"
#include "/home/breno/Github/brenocq-atta/ants/src/antComponent.h"
#include "/home/breno/Github/brenocq-atta/ants/src/world.h"
#include "/home/breno/Github/brenocq-atta/ants/src/worldComponent.h"

namespace atta::processor {

CpuSerial::CpuSerial() : Processor(Type::CPU_SERIAL) {}

CpuSerial::~CpuSerial() {}

void CpuSerial::startThread() { _thread = std::thread(&CpuSerial::loop, this); }

void CpuSerial::loop() {
    World world;

    cmp::Entity antPrototype = cmp::Entity(1);
    uint32_t num = antPrototype.get<cmp::Prototype>()->maxClones;
    cmp::EntityId first = 2;
    cmp::EntityId last = first + num - 1;

    float dt = processor::getDt();

    world.onStart(); // script::WorldRegistry::onStart();

    auto start = std::chrono::high_resolution_clock::now();
    while (shouldRun()) {
        // physics::update(dt);
        // sensor::update(dt);

        // script::WorldRegistry::onUpdateBefore();
        world.onUpdateBefore();

        for (cmp::EntityId e = first; e <= last; e++) {
            Ant ant;
            ant.entity = e;
            ant.update();
        }

        // std::vector<component::Entity> entities = component::getScriptView();
        // for (component::Entity entity : entities) {
        //     component::Script* script = entity.get<component::Script>();
        //     if (script) {
        //         const script::ControllerRegistry* controller = script::ControllerRegistry::getRegistry(script->sid);
        //         controller->update(entity);
        //     }
        // }

        // script::WorldRegistry::onUpdateAfter();
        world.onUpdateAfter();

        _stepCount++;
        if (_stepCount == 1000) {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            LOG_DEBUG("CpuSerial", "$0 steps in [y]$1ms", _stepCount, duration.count());
        }
    }
    // script::WorldRegistry::onStop();
    world.onStop();
    _stepCount = 0;
}

} // namespace atta::processor
