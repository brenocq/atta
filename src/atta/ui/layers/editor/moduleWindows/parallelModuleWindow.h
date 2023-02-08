//--------------------------------------------------
// Atta UI Module
// parallelModuleWindow.h
// Date: 2023-02-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_LAYERS_EDITOR_WINDOWS_PARALLEL_MODULE_WINDOW_H
#define ATTA_UI_LAYERS_EDITOR_WINDOWS_PARALLEL_MODULE_WINDOW_H

#include <atta/ui/layers/editor/window.h>

namespace atta::ui {

class ParallelModuleWindow : public Window<ParallelModuleWindow> {
  private:
    ParallelModuleWindow();
    void renderImpl();

    friend Window<ParallelModuleWindow>;
};

} // namespace atta::ui

#endif // ATTA_UI_LAYERS_EDITOR_WINDOWS_PARALLEL_MODULE_WINDOW_H
