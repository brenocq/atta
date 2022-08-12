//--------------------------------------------------
// Atta UI Module
// entityWindow.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_LAYERS_EDITOR_WINDOWS_ENTITY_WINDOW_H
#define ATTA_UI_LAYERS_EDITOR_WINDOWS_ENTITY_WINDOW_H

#include <atta/component/entity.h>
#include <atta/component/interface.h>

namespace atta::ui {

class EntityWindow {
  public:
    EntityWindow();

    void render();

  private:
    void renderTree();
    void renderTreeNode(component::EntityId entity, int& i);

    void renderComponents();
    void renderCameraWindows();
    void textureCombo(std::string comboId, StringId& sid);

    std::unordered_set<component::EntityId> _cameraWindows;

    // TODO Move this to undo/redo code
    std::vector<component::EntityId> _entitiesToDelete;
    std::vector<component::EntityId> _entitiesToCopy;
};

} // namespace atta::ui

#endif // ATTA_UI_LAYERS_EDITOR_WINDOWS_ENTITY_WINDOW_H
