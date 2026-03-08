// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_UI_INTERFACE_H
#define ATTA_UI_INTERFACE_H

#include <atta/component/interface.h>
#include <atta/ui/windows/viewport/viewport.h>

namespace atta::ui {

void startUp();
void shutDown();

//----- Custom component UI -----//
using ComponentUIFunc = std::function<void(cmp::Entity, cmp::Component*)>;
template <typename T>
void registerComponentUI(const ComponentUIFunc& renderFunc);
void registerComponentUI(cmp::ComponentId cid, const ComponentUIFunc& renderFunc);
template <typename T>
std::optional<ComponentUIFunc> getComponentUI();
std::optional<ComponentUIFunc> getComponentUI(cmp::ComponentId cid);

//----- Viewport -----//
const std::vector<std::shared_ptr<ui::Viewport>>& getViewports();
void openViewportModal(StringId sid);
void clearViewports();
void addViewport(std::shared_ptr<ui::Viewport> viewport);

bool getViewportRendering();
void setViewportRendering(bool viewportRendering);
unsigned getViewportDockId();

} // namespace atta::ui

#include <atta/ui/interface.inl>

#endif // ATTA_UI_INTERFACE_H
