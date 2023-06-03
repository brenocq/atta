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
    // This event will tell the UI to update the script combo boxes
    event::ScriptTarget evt;
    evt.scriptSids = getScriptSids();
    event::publish(evt);

    event::subscribe<event::ProjectOpen>(BIND_EVENT_FUNC(Manager::onProjectOpen));
    event::subscribe<event::ProjectClose>(BIND_EVENT_FUNC(Manager::onProjectClose));
}

void Manager::shutDownImpl() {}

void Manager::onProjectOpen(event::Event& event) {
    // Make sure to call onLoad only after the project is deserialized
    SystemRegistry::onLoad();
}

void Manager::onProjectClose(event::Event& event) {
    // Unload world script when project is closed
    SystemRegistry::onUnload();
}

} // namespace atta::script
