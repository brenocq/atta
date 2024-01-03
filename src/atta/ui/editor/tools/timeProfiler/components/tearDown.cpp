//--------------------------------------------------
// Atta UI Module
// tearDown.h
// Date: 2022-10-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/editor/tools/timeProfiler/components/tearDown.h>

#include <atta/ui/editor/widgets/help.h>

#include "imgui.h"

namespace atta::ui {

TearDown::TearDown() : _exclusive(true) {}

void TearDown::compute() {
    std::unordered_map<StringId, Profiler::Time> funcTimeDt;
    if (!_exclusive) {
        // Compute exclusive time
        for (auto& [name, records] : Profiler::calcRecordsByName(_lastRecordsSize)) {
            Profiler::Time time{};
            for (const Profiler::Record& r : records)
                time += r.end - r.begin;
            funcTimeDt[name] += time;
        }
    } else {
        // Compute time without nested functions
        for (auto& [threadId, records] : Profiler::calcRecordsByThreadId(_lastRecordsSize)) {
            std::vector<TimeInterval>& nestedTime = _nestedTime[threadId];
            // For each record in this thread
            for (size_t i = 0; i < records.size(); i++) {
                Profiler::Record r = records[i];

                // Total time
                Profiler::Time time = r.end - r.begin;

                // Remove time of nested functions
                for (int j = (int)nestedTime.size() - 1; j >= 0; j--) {
                    if (nestedTime[j].end < r.begin)
                        break;
                    else {
                        time -= (nestedTime[j].end - nestedTime[j].begin);
                        nestedTime.pop_back();
                    }
                }

                // If it is a nested function, add to nestedTime list
                if (r.name != SID("void atta::Atta::loop()"))
                    nestedTime.push_back({r.begin, r.end});

                // Add function time
                funcTimeDt[r.name] += time;
            }
        }
    }
    _lastRecordsSize = Profiler::getRecords().size();

    // Update _funcTime with funcTimeDt
    for (auto& [name, time] : funcTimeDt) {
        bool found = false;
        for (FuncTime& f : _funcTime)
            if (f.name == name) {
                f.time += time;
                found = true;
                break;
            }
        if (!found)
            _funcTime.push_back({name, time});
    }

    // Order by duration
    std::sort(_funcTime.begin(), _funcTime.end(), [](const FuncTime& a, const FuncTime& b) -> bool { return a.time > b.time; });

    // Update _totalTime/_maxTime
    _maxTime = 0;
    for (FuncTime f : _funcTime)
        if (f.time > _maxTime)
            _maxTime = f.time;
    _totalTime = Profiler::getTotalTime();
}

void TearDown::reset() {
    _funcTime.clear();
    _nestedTime.clear();
    _lastRecordsSize = 0;
}

void TearDown::render() {
    // Recorded data
    const std::vector<Profiler::Record>& records = Profiler::getRecords();

    // Exclusive option
    bool exclusiveChanged = false;
    if (ImGui::Checkbox("Exclusive", &_exclusive))
        exclusiveChanged = true;
    ImGui::SameLine();
    ui::help("Set to exclude the time of nested function calls");

    //----- Check for updates -----//
    static size_t lastRecordBegin = 0;
    static size_t lastRecordEnd = 0;
    size_t currRecordBegin = records.size() ? records.front().begin : 0;
    size_t currRecordEnd = records.size() ? records.back().end : 0;

    // If new recording or recordings were deleted
    if (currRecordBegin != lastRecordBegin || exclusiveChanged)
        reset();

    // If more recordings were added
    if ((currRecordEnd != lastRecordEnd && records.size()) || exclusiveChanged)
        compute();

    lastRecordBegin = currRecordBegin;
    lastRecordEnd = currRecordEnd;

    //----- Render table -----//
    static ImGuiTableFlags flags =
        ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable;
    static float textHeight = ImGui::GetTextLineHeightWithSpacing();
    ImVec2 size = ImVec2(0.0f, textHeight * 11);

    if (ImGui::BeginTable("TimeProfilerTearDown", 3, flags, size)) {
        // Header
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("00:00:00.000").x);
        ImGui::TableSetupColumn("Percentage");
        ImGui::TableHeadersRow();

        // Content
        if (_funcTime.size()) {
            ImGuiListClipper clipper;
            clipper.Begin(_funcTime.size());
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            while (clipper.Step()) {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
                    ImGui::TableNextRow();
                    const bool cropName = true;

                    // Name
                    ImGui::TableNextColumn();
                    std::string name = _funcTime[row].name.getString();
                    if (cropName)
                        name = Profiler::cropFuncName(name);
                    ImGui::Text(name.c_str());

                    // Time
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", Profiler::getTimeString(_funcTime[row].time).c_str());

                    // Total
                    ImGui::TableNextColumn();
                    ImGui::Text("%5.2f%%", _funcTime[row].time / float(_totalTime) * 100);
                    static const float sizePerc = ImGui::CalcTextSize("00.00% ").x;

                    float gap = 2.0f;
                    ImVec2 pMin = ImGui::GetCursorScreenPos();
                    pMin.x += sizePerc;
                    pMin.y += -textHeight;

                    ImVec2 pMax;
                    pMax.x = ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x;
                    pMax.y = pMin.y + textHeight;

                    pMin.x += gap;
                    pMin.y += gap;
                    pMax.x -= gap;
                    pMax.y -= gap;
                    float barPerc = _funcTime[row].time / float(_maxTime);
                    pMax.x = pMin.x + (pMax.x - pMin.x) * barPerc;

                    if (pMax.x > pMin.x) {
                        uint8_t r, g, b;
                        Profiler::getFuncColor(_funcTime[row].name, r, g, b);
                        drawList->AddRectFilled(pMin, pMax, IM_COL32(r, g, b, 255));
                    }
                }
            }
        }

        ImGui::EndTable();
    }
}

} // namespace atta::ui
