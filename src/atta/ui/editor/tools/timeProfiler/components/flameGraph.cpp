//--------------------------------------------------
// Atta UI Module
// flameGraph.h
// Date: 2022-10-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/editor/tools/timeProfiler/components/flameGraph.h>

#include "implot.h"

namespace atta::ui {

void FlameGraph::compute() {
    for (auto& [threadId, records] : Profiler::calcRecordsByThreadId(_lastRecordsSize)) {
        for (size_t i = 0; i < records.size(); i++) {
        }
    }
    _lastRecordsSize = Profiler::getRecords().size();
}

void FlameGraph::reset() {
    _flameBar = {};
    _lastRecordsSize = 0;
}

void FlameGraph::render() {
    // Recorded data
    const std::vector<Profiler::Record>& records = Profiler::getRecords();

    //----- Check for updates -----//
    static size_t lastRecordBegin = 0;
    static size_t lastRecordEnd = 0;
    size_t currRecordBegin = records.size() ? records.front().begin : 0;
    size_t currRecordEnd = records.size() ? records.back().end : 0;

    // If new recording or recordings were deleted
    if (currRecordBegin != lastRecordBegin)
        reset();

    // If more recordings were added
    if (currRecordEnd != lastRecordEnd && records.size())
        compute();

    lastRecordBegin = currRecordBegin;
    lastRecordEnd = currRecordEnd;

    //----- Render flame graph -----//
    if (ImPlot::BeginPlot("##FlameGraph")) {
        ImVec2 cntr = ImPlot::PlotToPixels(ImPlotPoint(0.5f, 0.5f));
        ImVec2 rmin = ImPlot::PlotToPixels(ImPlotPoint(0.25f, 0.75f));
        ImVec2 rmax = ImPlot::PlotToPixels(ImPlotPoint(0.75f, 0.25f));
        ImPlot::PushPlotClipRect();
        if (_flameBar.name != StringId()) {
            ImPlot::GetPlotDrawList()->AddCircleFilled(cntr, 20, IM_COL32(255, 255, 0, 255), 20);
            ImPlot::GetPlotDrawList()->AddRect(rmin, rmax, IM_COL32(128, 0, 255, 255));
        }
        ImPlot::PopPlotClipRect();
        ImPlot::EndPlot();
    }
}

} // namespace atta::ui
