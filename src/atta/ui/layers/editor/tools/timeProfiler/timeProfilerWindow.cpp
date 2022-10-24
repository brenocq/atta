//--------------------------------------------------
// Atta UI Module
// timeProfilerWindow.cpp
// Date: 2022-10-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/layers/editor/tools/timeProfiler/timeProfilerWindow.h>
#include <atta/ui/layers/editor/components/button.h>

namespace atta::ui {

TimeProfilerWindow::TimeProfilerWindow() {
    setName("Time Profiler");
    _initialSize = vec2(500.0f, 700.0f);
}

void TimeProfilerWindow::renderImpl() {
    const float buttonSize = ImGui::GetTextLineHeightWithSpacing();

    if (!Profiler::isRecording()) {
        if (renderButton("play", buttonSize))
            Profiler::startRecording();
    } else {
        if (renderButton("stop", buttonSize))
            Profiler::stopRecording();
    }

    if (ImGui::CollapsingHeader("Flame Graph")) {
        ImGui::Text("Cool flame graph");
    }

    if (ImGui::CollapsingHeader("Tear Down"))
        _tearDown.render();

    if (ImGui::CollapsingHeader("Timeline")) {
        ImGui::Text("Cool timeline");
    }
}

} // namespace atta::ui
