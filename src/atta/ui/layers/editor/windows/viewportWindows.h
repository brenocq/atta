//--------------------------------------------------
// Atta UI Module
// viewportWindows.h
// Date: 2021-12-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_LAYERS_EDITOR_WINDOWS_VIEWPORT_WINDOWS_H
#define ATTA_UI_LAYERS_EDITOR_WINDOWS_VIEWPORT_WINDOWS_H

#include <atta/component/entity.h>
#include <atta/component/manager.h>

namespace atta::ui {

class ViewportWindows {
  public:
    void render();

  private:
    void addBasicShapePopup();
};

} // namespace atta::ui

#endif // ATTA_UI_LAYERS_EDITOR_WINDOWS_VIEWPORT_WINDOWS_H
