//--------------------------------------------------
// Atta UI Module
// processorModuleWindow.h
// Date: 2023-02-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_LAYERS_EDITOR_WINDOWS_PROCESSOR_MODULE_WINDOW_H
#define ATTA_UI_LAYERS_EDITOR_WINDOWS_PROCESSOR_MODULE_WINDOW_H

#include <atta/ui/layers/editor/window.h>

namespace atta::ui {

class ProcessorModuleWindow : public Window<ProcessorModuleWindow> {
  private:
    ProcessorModuleWindow();
    void renderImpl();

    friend Window<ProcessorModuleWindow>;
};

} // namespace atta::ui

#endif // ATTA_UI_LAYERS_EDITOR_WINDOWS_PROCESSOR_MODULE_WINDOW_H
