//--------------------------------------------------
// Atta Script Module
// managerStatic.cpp
// Date: 2022-06-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/scripts.h>

namespace atta::script {

void Manager::startUpImpl() {
    // Publish registered scripts. Need to do here because scripts are
    // registered before ComponentSystem::startUp())
    event::ScriptTarget evt;
    evt.scriptSids = getScriptSids();
    event::publish(evt);

    if(_projectScript.second)
        _projectScript.second->onLoad();
}

void Manager::shutDownImpl() {
    if (_projectScript.second != nullptr)
        delete _projectScript.second;

    for (auto& [key, s] : _scripts)
        delete s;
    _scripts.clear();
}

} // namespace atta::script
