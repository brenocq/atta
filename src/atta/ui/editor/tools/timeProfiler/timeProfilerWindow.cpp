//--------------------------------------------------
// Atta UI Module
// timeProfilerWindow.cpp
// Date: 2022-10-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/editor/tools/timeProfiler/timeProfilerWindow.h>
#include <atta/ui/widgets/button.h>

namespace atta::ui {

TimeProfilerWindow::TimeProfilerWindow() {
    setName("Time Profiler");
    _initialSize = vec2(500.0f, 700.0f);
}

void TimeProfilerWindow::renderImpl() {
    const float buttonSize = ImGui::GetTextLineHeightWithSpacing();

    size_t size = Profiler::getRecords().size();
    if(size)
    {
        ImGui::Text("%u events", unsigned(size));
        ImGui::SameLine();
    }

    if (!Profiler::isRecording()) {
        if (ui::imageButton("icons/play.png", buttonSize))
            Profiler::startRecording();
    } else {
        if (ui::imageButton("icons/stop.png", buttonSize))
            Profiler::stopRecording();
    }

    if(size)
    {
        ImGui::SameLine();
        ImGui::Text("%s", Profiler::getTimeString(Profiler::getTotalTime()).c_str());
    }

    //if (ImGui::CollapsingHeader("Flame Graph"))
    //    _flameGraph.render();

    if (ImGui::CollapsingHeader("Tear Down"))
        _tearDown.render();

    if (ImGui::CollapsingHeader("Timeline"))
        _timeline.render();
}

} // namespace atta::ui
