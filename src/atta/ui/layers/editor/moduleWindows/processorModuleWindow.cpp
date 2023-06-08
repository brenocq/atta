//--------------------------------------------------
// Atta UI Module
// processorModuleWindow.cpp
// Date: 2023-02-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/layers/editor/moduleWindows/processorModuleWindow.h>

#include <atta/processor/interface.h>

namespace atta::ui {

ProcessorModuleWindow::ProcessorModuleWindow() {
    setName("Processor Module");
    _initialSize = vec2(250.0f, 300.0f);
}

void ProcessorModuleWindow::renderImpl() {
    // Processor selection
    ImGui::Text("Processor");
    ImGui::SameLine();
    std::vector<const char*> devices = {"CPU Serial", "CPU Parallel", "GPU", "Cluster"};
    int selected = int(processor::getType());
    if (ImGui::Combo("##ProcessorProcessor", &selected, devices.data(), devices.size()))
        if (selected != int(processor::getType()))
            processor::setType(processor::Type(selected));

    ImGui::Separator();

    // Processor specific options
    switch (processor::Type(selected)) {
        case processor::Type::CPU_SERIAL:
            break;
        case processor::Type::CPU_PARALLEL: {
            auto p = processor::getCpuParallelProcessor();
            int numThreads = p->getNumWorkers();
            int maxThreads = p->getMaxNumWorkers();
            ImGui::SetNextItemWidth(100.0f);
            if (ImGui::SliderInt("Number of threads", &numThreads, 1, maxThreads))
                p->setNumWorkers(numThreads);
            break;
        }
        case processor::Type::GPU:
            break;
        case processor::Type::CLUSTER:
            break;
    }
}

} // namespace atta::ui
