//--------------------------------------------------
// Atta Script Module
// managerStatic.cpp
// Date: 2022-06-12
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/scripts.h>
#include <atta/event/events/projectOpen.h>

namespace atta::script {

void Manager::startUpImpl() {
    // Publish registered scripts. Need to do here because scripts are
    // registered before ComponentSystem::startUp())
    // This event will make the UI script combo boxes be updated
    event::ScriptTarget evt;
    evt.scriptSids = getScriptSids();
    event::publish(evt);

    event::subscribe<event::ProjectOpen>(BIND_EVENT_FUNC(Manager::onProjectOpen));
}

void Manager::shutDownImpl() {
    if (_projectScript.second != nullptr)
        delete _projectScript.second;

    for (auto& [key, s] : _scripts)
        delete s;
    _scripts.clear();
}

void Manager::onProjectOpen(event::Event& event) {
    // Make sure to call onLoad only after the project is deserialized
    if (_projectScript.second)
        _projectScript.second->onLoad();
}

} // namespace atta::script
