//--------------------------------------------------
// Atta Script Module
// managerStatic.cpp
// Date: 2022-06-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/scripts.h>

namespace atta::script {
void Manager::startUp() { getInstance().startUpImpl(); }
void Manager::startUpImpl() {
    // Publish registered scripts. Need to do here because scripts are
    // registered before ComponentSystem::startUp())
    ScriptTargetEvent evt;
    evt.scriptSids = getScriptSids();
    EventManager::publish(evt);
}

void Manager::shutDown() { getInstance().shutDownImpl(); }
void Manager::shutDownImpl() {
    if (_projectScript.second != nullptr)
        delete _projectScript.second;

    for (auto& [key, s] : _scripts)
        delete s;
    _scripts.clear();
}

ProjectScript* Manager::registerProjectScript(std::string name, ProjectScript* projectScript) {
    LOG_VERBOSE("Manager", "Registering project script [w]$0[]", name);
    getInstance()._projectScript.first = StringId(name);
    getInstance()._projectScript.second = projectScript;

    return projectScript;
}

Script* Manager::registerScript(std::string name, Script* script) {
    LOG_VERBOSE("Manager", "Registering script [w]$0[]", name);
    getInstance()._scripts[StringId(name)] = script;

    return script;
}
} // namespace atta::script
