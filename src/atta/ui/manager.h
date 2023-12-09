//--------------------------------------------------
// Atta UI Module
// manager.h
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_MANAGER_H
#define ATTA_UI_MANAGER_H

#include <atta/ui/interface.h>
#include <atta/ui/editor/editor.h>

namespace atta::ui {

class Manager final {
  public:
    static Manager& getInstance();

    friend void startUp();
    friend void shutDown();

  private:
    void startUpImpl();
    void shutDownImpl();

    void setTheme();
    void initOpenGL();
    void initVulkan();

    void render();
    Editor _editor;
};

} // namespace atta::ui

#endif // ATTA_UI_MANAGER_H
