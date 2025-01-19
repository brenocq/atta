//--------------------------------------------------
// Atta UI Module
// interface.cpp
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/interface.h>
#include <atta/ui/manager.h>

namespace atta::ui {

void startUp() { Manager::getInstance().startUpImpl(); }
void shutDown() { Manager::getInstance().shutDownImpl(); }

//----- Viewport -----//
const std::vector<std::shared_ptr<ui::Viewport>>& getViewports() { return Manager::getInstance().getViewportsImpl(); }
void openViewportModal(StringId sid) { Manager::getInstance().openViewportModalImpl(sid); }
void addViewport(std::shared_ptr<ui::Viewport> viewport) { Manager::getInstance().addViewportImpl(viewport); }

bool getViewportRendering() { return Manager::getInstance().getViewportRenderingImpl(); }
void setViewportRendering(bool viewportRendering) { Manager::getInstance().setViewportRenderingImpl(viewportRendering); }

} // namespace atta::ui
