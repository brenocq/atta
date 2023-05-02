//--------------------------------------------------
// Atta UI Module
// memoryModuleWindow.h
// Date: 2023-05-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_LAYERS_EDITOR_WINDOWS_MEMORY_MODULE_WINDOW_H
#define ATTA_UI_LAYERS_EDITOR_WINDOWS_MEMORY_MODULE_WINDOW_H

#include <atta/ui/layers/editor/window.h>

#include <atta/memory/interface.h>

namespace atta::ui {

class MemoryModuleWindow : public Window<MemoryModuleWindow> {
  private:
    MemoryModuleWindow();
    void renderImpl();

    void renderAllocatorTree(mem::Allocator* allocator);
    std::unordered_map<mem::Allocator*, mem::Allocator*> _parentMap;

    friend Window<MemoryModuleWindow>;
};

} // namespace atta::ui

#endif // ATTA_UI_LAYERS_EDITOR_WINDOWS_MEMORY_MODULE_WINDOW_H
