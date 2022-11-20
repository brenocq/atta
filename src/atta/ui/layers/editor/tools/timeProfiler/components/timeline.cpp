//--------------------------------------------------
// Atta UI Module
// timeline.h
// Date: 2022-10-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/layers/editor/tools/timeProfiler/components/timeline.h>

#include "implot.h"

namespace atta::ui {

void Timeline::compute() {
    for (auto& [threadId, records] : Profiler::calcRecordsByThreadId(_lastRecordsSize)) {
        std::vector<Profiler::Record>& sortedRecords = _sortedRecords[threadId];
        for (int i = 0; i < records.size(); i++) {
            // Add to back
            sortedRecords.push_back(records[i]);

            // Move back until correct position
            for (int j = sortedRecords.size() - 2; j >= 0; j--) {
                if (sortedRecords[j].begin > sortedRecords[j + 1].begin)
                    std::swap(sortedRecords[j], sortedRecords[j + 1]);
                else
                    break;
            }
        }
    }
    _lastRecordsSize = Profiler::getRecords().size();

    // std::unordered_map<Profiler::ThreadId, std::stack<Profiler::Record>> _currStack;
    // std::unordered_map<Profiler::ThreadId, size_t> _maxStackSize;
}

void Timeline::reset() {
    _sortedRecords.clear();
    _lastRecordsSize = 0;
}

void Timeline::render() {
    // Recorded data
    const std::vector<Profiler::Record>& records = Profiler::getRecords();
    bool shouldAutoFit = false;

    //----- Check for updates -----//
    static size_t lastRecordBegin = 0;
    static size_t lastRecordEnd = 0;
    size_t currRecordBegin = records.size() ? records.front().begin : 0;
    size_t currRecordEnd = records.size() ? records.back().end : 0;

    // If new recording or recordings were deleted
    if (currRecordBegin != lastRecordBegin)
        reset();

    // If more recordings were added
    if (currRecordEnd != lastRecordEnd && records.size()) {
        compute();
        shouldAutoFit = true;
    }

    lastRecordBegin = currRecordBegin;
    lastRecordEnd = currRecordEnd;

    //----- Render timeline -----//
    constexpr Profiler::Time toSec = Profiler::ticksPerSecond;
    constexpr float recordingShowInterval = 0.100f; // Interval in seconds to show last recordings while recording
    Profiler::Time start = Profiler::getStart();

    float plotStart = 0.0f;
    float plotEnd = (currRecordEnd - start) / float(toSec);

    if ((plotEnd - plotStart) > recordingShowInterval)
        plotStart = plotEnd - recordingShowInterval;

    for (const auto& [thread, records] : _sortedRecords) {
        std::stack<Profiler::Record> s;
        static size_t maxStackSize = 1;
        // Keep track of last draw position to avoid rendering more than one rect to the same pixel
        std::vector<float> lastDrawTimePerLevel;
        if (ImPlot::BeginPlot(("##Timeline" + std::to_string(thread)).c_str())) {
            ImPlotCond autoFitCond = shouldAutoFit ? ImPlotCond_Always : ImPlotCond_None;

            ImPlot::SetupAxisLimits(ImAxis_X1, plotStart, plotEnd, autoFitCond);
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0f, maxStackSize, ImPlotCond_Always);

            ImPlot::SetupAxis(ImAxis_X1, "Time");
            ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_Invert);

            ImPlotRect limits = ImPlot::GetPlotLimits();
            ImVec2 size = ImVec2(limits.X.Max - limits.X.Min, limits.Y.Max - limits.Y.Min);
            ImVec2 sizePixels = ImPlot::GetPlotSize();
            float pixelTime = size.x / sizePixels.x; // Time interval of each pixel

            ImPlot::PushPlotClipRect();
            float nextDrawTime = 0;
            maxStackSize = 0;
            for (const Profiler::Record& record : records) {
                // Free stack
                while (!s.empty() && s.top().end < record.begin)
                    s.pop();
                lastDrawTimePerLevel.resize(s.size() + 1);

                // Draw rectangle
                float beginTime = (record.begin - start) / float(toSec);
                float endTime = (record.end - start) / float(toSec);
                bool isInside = !(beginTime > limits.X.Max || endTime < limits.X.Min); /// XXX can save begin/end for next draw
                bool bigEnough = endTime >= lastDrawTimePerLevel[s.size()];

                // Render if it will be rendered
                if (isInside && bigEnough) {
                    endTime = std::max(beginTime + pixelTime * 0.5f, endTime);
                    lastDrawTimePerLevel[s.size()] = endTime;

                    //----- Render rect -----//
                    ImVec2 min = ImPlot::PlotToPixels(ImPlotPoint(beginTime, float(s.size())));
                    ImVec2 max = ImPlot::PlotToPixels(ImPlotPoint(endTime, float(s.size() + 1)));
                    uint8_t r, g, b;
                    Profiler::getFuncColor(record.name, r, g, b);
                    ImPlot::GetPlotDrawList()->AddRectFilled(min, max, IM_COL32(r, g, b, 255));

                    //----- Render name -----//
                    std::string name = record.name.getString();
                    name = Profiler::cropFuncName(name);
                    // Calculate visible min/max
                    min.x = std::max(beginTime, float(limits.X.Min));
                    max.x = std::min(endTime, float(limits.X.Max));
                    // Calculate text size
                    ImPlotPoint textOrigin = ImPlot::PixelsToPlot(ImVec2(0,0));
                    ImPlotPoint textSize = ImPlot::PixelsToPlot(ImGui::CalcTextSize(name.c_str()));
                    textSize.x = textSize.x - textOrigin.x;
                    // If text too big, try funcName...
                    if (textSize.x >= max.x - min.x) {
                        // If can't render full name, try partial name
                        std::string fullName = name;
                        for(int s = 5; s <= fullName.size()-3; s++)
                        {
                            std::string subStr = fullName.substr(0, s) + "...";
                            ImPlotPoint subTextSize = ImPlot::PixelsToPlot(ImGui::CalcTextSize(subStr.c_str()));
                            subTextSize.x = subTextSize.x - textOrigin.x;
                            if (subTextSize.x < max.x - min.x)
                            {
                                name = subStr;
                                textSize = subTextSize;
                            }
                            else
                                break;
                        }
                    }

                    // Render text if possible
                    if (textSize.x < max.x - min.x) {
                        float x = (min.x + max.x) * 0.5f;
                        float y = s.size() + 0.5f;
                        ImPlot::PlotText(name.c_str(), x, y);
                    }
                }

                // Check stop rendering
                if (beginTime > limits.X.Max)
                    break;

                // Add to stack
                s.push(record);
                if (isInside)
                    maxStackSize = std::max(maxStackSize, s.size());
            }
            ImPlot::PopPlotClipRect();

            ImPlot::EndPlot();
        }
    }
}

} // namespace atta::ui
