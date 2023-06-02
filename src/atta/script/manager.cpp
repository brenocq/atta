//--------------------------------------------------
// Atta Script Module
// manager.cpp
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/dummy.h>
#include <atta/script/interface.h>
#include <atta/script/manager.h>

#include <atta/event/events/projectClose.h>
#include <atta/event/events/projectOpen.h>
#include <atta/event/events/scriptTarget.h>
#include <atta/event/interface.h>

namespace atta::script {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
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
