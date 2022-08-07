//--------------------------------------------------
// Atta UI Module
// manager.cpp
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/manager.h>
#include <atta/ui/layers/editor/editorLayer.h>
#include <atta/ui/layers/uiLayer.h>
#include <atta/ui/manager.h>

namespace atta::ui {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUp() { getInstance().startUpImpl(); }
void Manager::startUpImpl() {
    graphics::Manager::pushLayer(new EditorLayer());
    graphics::Manager::pushLayer(new UILayer());
}

void Manager::shutDown() { getInstance().shutDownImpl(); }
void Manager::shutDownImpl() {}

} // namespace atta::ui
