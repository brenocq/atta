//--------------------------------------------------
// Atta Script Module
// manager.cpp
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/dummy.h>
#include <atta/script/interface.h>
#include <atta/script/manager.h>

#include <atta/component/components/prototype.h>
#include <atta/component/components/script.h>
#include <atta/component/interface.h>
#include <atta/event/events/projectClose.h>
#include <atta/event/events/projectOpen.h>
#include <atta/event/events/scriptTarget.h>
#include <atta/event/interface.h>
#include <atta/parallel/interface.h>

namespace atta::script {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::updateImpl(float dt) {
    // Run world script before
    {
        PROFILE_NAME("atta::script::Manager::onUpdateBefore");
        WorldRegistry::onUpdateBefore(dt);
    }

    // Run clone scripts
    {
        PROFILE_NAME("atta::script::Manager::runCloneScripts");
        for (auto& factory : component::getFactories()) {
            cmp::Script* script = factory.getPrototype().get<cmp::Script>();
            if (script) {
                const ControllerRegistry* controller = ControllerRegistry::getRegistry(script->sid);
                controller->run(factory.getFirstClone(), dt, factory.getMaxClones());
            }
        }
    }

    // Run base entity scripts (not clones)
    {
        PROFILE_NAME("atta::script::Manager::runBaseScripts");

        // Get entityIds that are clones
        const auto& factories = component::getFactories();
        std::vector<std::pair<component::EntityId, component::EntityId>> beginEndClones(factories.size());
        for (const auto& factory : factories) {
            component::EntityId firstClone = factory.getFirstClone().getId();
            uint64_t numClones = factory.getNumEntitiesCloned() * factory.getMaxClones();
            beginEndClones.push_back({firstClone, firstClone + numClones});
        }

        std::vector<component::Entity> entities = component::getScriptView();
        // std::vector<std::pair<script::Script*, cmp::EntityId>> scripts;
        // for (component::EntityId entity : entities) {
        //     // Check if it has script component
        //     component::Script* scriptComponent = component::getComponent<component::Script>(entity);
        //     if (!scriptComponent)
        //         continue;

        //    // Check if it is not prototype entity
        //    component::Prototype* prototypeComponent = component::getComponent<component::Prototype>(entity);
        //    if (prototypeComponent)
        //        continue;

        //    // Check if it it not clone entity
        //    bool isClone = false;
        //    for (auto [begin, end] : beginEndClones)
        //        if (entity >= begin && entity <= end) {
        //            isClone = true;
        //            break;
        //        }
        //    if (isClone)
        //        continue;

        //    // Add to list to be executed
        //    // script::Script* script = getScriptImpl(scriptComponent->sid);
        //    // if (script)
        //    //    scripts.push_back({script, entity});
        //}

        //// Run scripts
        // if (scripts.size())
        //     parallel::run(0, scripts.size() - 1, [&](uint32_t i) { scripts[i].first->update(component::Entity(scripts[i].second), dt); });

        ControllerRegistry::getRegistries().at(0)->run(-1, 0.0f);
    }

    // Run project script after
    {
        PROFILE_NAME("atta::script::Manager::onUpdateAfter");
        WorldRegistry::onUpdateAfter(dt);
    }
}

std::vector<StringId> Manager::getScriptSidsImpl() const {
    std::vector<const ControllerRegistry*> registries = ControllerRegistry::getRegistries();
    std::vector<StringId> scripts;
    for (const ControllerRegistry* r : registries)
        scripts.push_back(r->getName());
    return scripts;
}

} // namespace atta::script

#ifdef ATTA_STATIC_PROJECT
#include <atta/script/managerStatic.cpp>
#else
#include <atta/script/managerDynamic.cpp>
#endif
