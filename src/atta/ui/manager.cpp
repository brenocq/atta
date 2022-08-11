//--------------------------------------------------
// Atta UI Module
// manager.cpp
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/interface.h>
#include <atta/ui/layers/editor/editorLayer.h>
#include <atta/ui/layers/uiLayer.h>
#include <atta/ui/manager.h>

namespace atta::ui {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUpImpl() {
    graphics::pushLayer(new EditorLayer());
    graphics::pushLayer(new UILayer());
}

void Manager::shutDownImpl() {}

} // namespace atta::ui
