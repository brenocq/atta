//--------------------------------------------------
// Atta UI Module
// parallelModuleWindow.cpp
// Date: 2023-02-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/layers/editor/moduleWindows/parallelModuleWindow.h>

#include <atta/parallel/interface.h>

namespace atta::ui {

ParallelModuleWindow::ParallelModuleWindow() {
    setName("Parallel Module");
    _initialSize = vec2(250.0f, 300.0f);
}

void ParallelModuleWindow::renderImpl() {
    // Device selection
    ImGui::Text("Device");
    ImGui::SameLine();
    std::vector<const char*> devices = {"Serial", "CPU", "GPU", "Cluster"};
    int selected = int(parallel::getDeviceType());
    if (ImGui::Combo("##ParallelDevice", &selected, devices.data(), devices.size()))
        if (selected != int(parallel::getDeviceType()))
            parallel::setDeviceType(parallel::Device::Type(selected));

    ImGui::Separator();

    // Device specific options
    switch (parallel::Device::Type(selected)) {
        case parallel::Device::Type::SERIAL:
            break;
        case parallel::Device::Type::CPU: {
            int numThreads = parallel::getCpuDevice()->getNumWorkers();
            int maxThreads = parallel::getCpuDevice()->getMaxNumWorkers();
            ImGui::SetNextItemWidth(100.0f);
            if (ImGui::SliderInt("Number of threads", &numThreads, 1, maxThreads))
                parallel::getCpuDevice()->setNumWorkers(numThreads);
            break;
        }
        case parallel::Device::Type::GPU:
            break;
        case parallel::Device::Type::CLUSTER:
            break;
    }
}

} // namespace atta::ui
