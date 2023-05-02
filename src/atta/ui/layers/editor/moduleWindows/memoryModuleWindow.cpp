//--------------------------------------------------
// Atta UI Module
// memoryModuleWindow.cpp
// Date: 2023-02-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/layers/editor/moduleWindows/memoryModuleWindow.h>
#include <regex>

namespace atta::ui {

std::string formatSize(unsigned long long number, int factor = 1024) {
    static const char* units[] = {"", "K", "M", "G", "T", "P", "E", "Z", "Y"};
    static const int numUnits = sizeof(units) / sizeof(units[0]);

    double size = number;
    int idx = 0;
    while (size >= factor && idx < numUnits - 1) {
        size /= factor;
        idx++;
    }

    char buf[32];
    sprintf(buf, "%.2f%s", size, units[idx]);
    return std::string(buf);
}

MemoryModuleWindow::MemoryModuleWindow() {
    setName("Memory Module");
    _initialSize = vec2(250.0f, 300.0f);
}

void MemoryModuleWindow::renderAllocatorTree(mem::Allocator* allocator) {
    // TODO pretty name
    std::string name = mem::getAllocatorName(allocator);

    std::string size = formatSize(allocator->getSize()) + "B";

    // Find children
    std::vector<mem::Allocator*> children;
    auto allocators = mem::getAllocators();
    for (auto& [child, parent] : _parentMap)
        if (parent == allocator)
            children.push_back(child);

    // Flags
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
    if (children.empty())
        flags |= ImGuiTreeNodeFlags_Leaf;

    // Render node
    if (ImGui::TreeNodeEx(name.c_str(), flags, "%s (%s)", name.c_str(), size.c_str())) {
        // Render children
        for (auto child : children)
            renderAllocatorTree(child);

        ImGui::TreePop();
    }
}

void MemoryModuleWindow::renderImpl() {
    // Build parent map
    _parentMap.clear();
    auto allocators = mem::getAllocators();
    for (auto& [hash, allocator] : allocators)
        _parentMap[allocator] = nullptr;

    for (auto& [hash, parent] : allocators)
        for (auto& [childHash, child] : allocators)
            if (child != parent && parent->owns(child->getMemory()) && parent->getSize() > child->getSize() &&
                (_parentMap[child] == nullptr || _parentMap[child]->getSize() > parent->getSize()))
                _parentMap[child] = parent;

    // Render tree
    renderAllocatorTree(mem::getAllocator(SID("MainAllocator")));
}

} // namespace atta::ui
