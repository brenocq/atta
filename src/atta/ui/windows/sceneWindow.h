//--------------------------------------------------
// Atta UI Module
// sceneWindow.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_WINDOWS_SCENE_WINDOW_H
#define ATTA_UI_WINDOWS_SCENE_WINDOW_H

#include <atta/component/interface.h>
#include <atta/ui/windows/window.h>

namespace atta::ui {

class SceneWindow : public Window<SceneWindow> {
  private:
    SceneWindow();

    void renderImpl();

    void renderTree();
    void renderTreeNode(component::EntityId entity, int& i);

    void renderComponents();
    void textureCombo(std::string comboId, StringId& sid);

    // TODO Move this to undo/redo code
    std::vector<component::EntityId> _entitiesToDelete;
    std::vector<component::EntityId> _entitiesToCopy;

    friend Window<SceneWindow>;
};

} // namespace atta::ui

#endif // ATTA_UI_WINDOWS_SCENE_WINDOW_H
