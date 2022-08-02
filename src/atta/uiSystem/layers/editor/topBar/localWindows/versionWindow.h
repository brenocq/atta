//--------------------------------------------------
// Atta UI System
// versionWindow.h
// Date: 2022-06-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_SYSTEM_LAYERS_EDITOR_TOP_BAR_LOCAL_WINDOWS_VERSION_WINDOW_H
#define ATTA_UI_SYSTEM_LAYERS_EDITOR_TOP_BAR_LOCAL_WINDOWS_VERSION_WINDOW_H
#include <atta/uiSystem/layers/editor/window.h>
#include <atta/physicsSystem/physicsManager.h>

namespace atta::ui
{
    class VersionWindow : public Window<VersionWindow>
    {
    private:
        VersionWindow();
        void renderImpl();

        void checkInstalledVersions();
        std::set<std::string> _installedVersions;

        friend Window<VersionWindow>;
    };
}

#endif// ATTA_UI_SYSTEM_LAYERS_EDITOR_TOP_BAR_LOCAL_WINDOWS_VERSION_WINDOW_H