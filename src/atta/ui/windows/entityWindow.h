// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

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
