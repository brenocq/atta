// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_UI_WINDOWS_ENTITY_WINDOW_H
#define ATTA_UI_WINDOWS_ENTITY_WINDOW_H

#include <atta/component/interface.h>

namespace atta::ui {

class EntityWindow {
  public:
    void render();

  private:
    void renderTree();
    void renderTreeNode(component::EntityId entity, int& i);

    void renderComponents();
    void textureCombo(std::string comboId, StringId& sid);

    // TODO Move this to undo/redo code
    std::vector<component::EntityId> _entitiesToDelete;
    std::vector<component::EntityId> _entitiesToCopy;
};

} // namespace atta::ui

#endif // ATTA_UI_WINDOWS_ENTITY_WINDOW_H
