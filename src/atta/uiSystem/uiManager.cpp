//--------------------------------------------------
// Atta UI System
// uiManager.cpp
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/uiManager.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/uiSystem/layers/uiLayer.h>
#include <atta/uiSystem/layers/editor/editorLayer.h>

namespace atta::ui
{
    UIManager& UIManager::getInstance()
    {
        static UIManager instance;
        return instance;
    }

    void UIManager::startUp() { getInstance().startUpImpl(); }
    void UIManager::startUpImpl()
    {
        GraphicsManager::pushLayer(new EditorLayer());
        GraphicsManager::pushLayer(new UILayer());
    }

    void UIManager::shutDown() { getInstance().shutDownImpl(); }
    void UIManager::shutDownImpl()
    {

    }
}
