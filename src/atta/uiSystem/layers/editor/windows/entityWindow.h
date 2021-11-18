//--------------------------------------------------
// Atta UI System
// entityWindow.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOWS_ENTITY_WINDOW_H
#define ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOWS_ENTITY_WINDOW_H
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/entity.h>

namespace atta::ui
{
    class EntityWindow
    {
    public:
        EntityWindow();

        void render();
    private:
        void renderTree();
        void renderTreeNode(EntityId entity, int& i);

        void renderComponents();
        void renderCameraWindows();
        void textureCombo(std::string comboId, StringId& sid);

        std::unordered_set<EntityId> _cameraWindows;

        // TODO Move this to undo/redo code
        std::vector<EntityId> _entitiesToDelete;
        std::vector<EntityId> _entitiesToCopy;
    };
}

#endif// ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOWS_ENTITY_WINDOW_H
