//--------------------------------------------------
// Atta UI System
// viewportWindows.h
// Date: 2021-12-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOWS_VIEWPORT_WINDOWS_H
#define ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOWS_VIEWPORT_WINDOWS_H
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/entity.h>

namespace atta::ui
{
    class ViewportWindows
    {
    public:
        void render();

    private:
        void addBasicShapePopup();
    };
}

#endif// ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOWS_VIEWPORT_WINDOWS_H
