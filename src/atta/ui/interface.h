//--------------------------------------------------
// Atta UI Module
// interface.h
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_INTERFACE_H
#define ATTA_UI_INTERFACE_H

#include <atta/ui/windows/viewport/viewport.h>

namespace atta::ui {

void startUp();
void shutDown();

//----- Viewport -----//
const std::vector<std::shared_ptr<ui::Viewport>>& getViewports();
void openViewportModal(StringId sid);
void addViewport(std::shared_ptr<ui::Viewport> viewport);

bool getViewportRendering();
void setViewportRendering(bool viewportRendering);

} // namespace atta::ui

#endif // ATTA_UI_INTERFACE_H
