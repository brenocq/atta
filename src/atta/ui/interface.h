//--------------------------------------------------
// Atta UI Module
// interface.h
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_INTERFACE_H
#define ATTA_UI_INTERFACE_H

#include <atta/component/base.h>
#include <atta/component/components/component.h>
#include <atta/ui/windows/viewport/viewport.h>

namespace atta::ui {

void startUp();
void shutDown();

//----- Custom component UI -----//
using ComponentUIFunc = std::function<void(cmp::Component*)>;
void registerComponentUI(cmp::ComponentId cid, ComponentUIFunc renderFunc);
std::optional<ComponentUIFunc> getComponentUI(cmp::ComponentId cid);

//----- Viewport -----//
const std::vector<std::shared_ptr<ui::Viewport>>& getViewports();
void openViewportModal(StringId sid);
void addViewport(std::shared_ptr<ui::Viewport> viewport);

bool getViewportRendering();
void setViewportRendering(bool viewportRendering);
unsigned getViewportDockId();

} // namespace atta::ui

#endif // ATTA_UI_INTERFACE_H
