//--------------------------------------------------
// Atta UI Module
// tearDown.h
// Date: 2022-10-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/layers/editor/tools/timeProfiler/components/tearDown.h>

#include "imgui.h"

namespace atta::ui {

void TearDown::render() {
    static ImGuiTableFlags flags =
        ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable;
    static float textHeight = ImGui::GetTextLineHeightWithSpacing();
    static uint64_t lastRecordTime = 0;

    // Data to show
    static std::vector<std::pair<uint64_t, const char*>> functionTime; // Total execution time of each function
    static bool cumulative = true;
    bool cumulativeChanged = false;

    // Recorded data
    const std::vector<Profiler::Record>& records = Profiler::getRecords();

    if (ImGui::Checkbox("Cumulative", &cumulative))
        cumulativeChanged = true;

    // Check for updates
    if (records.size() && (records.back().end != lastRecordTime || cumulativeChanged)) {
        std::unordered_map<const char*, size_t> nameToIdx; // Map function name to idx in functionTime
        cumulativeChanged = false;

        lastRecordTime = records.back().end;
        nameToIdx.clear();
        functionTime.clear();

        // Cumulative compute time
        if (cumulative) {
            for (int i = 0; i < records.size(); i++) {
                Profiler::Record r = records[i];

                if (nameToIdx.find(r.name) == nameToIdx.end()) {
                    nameToIdx[r.name] = functionTime.size();
                    functionTime.push_back({r.end - r.begin, r.name});
                } else
                    functionTime[nameToIdx[r.name]].first += (r.end - r.begin);
            }
        } else {
            //std::stack<Profiler::Record> s;

        }

        // Order by duration
        std::sort(functionTime.begin(), functionTime.end(), std::greater<std::pair<uint64_t, const char*>>());
    }

    // Render table
    ImVec2 size = ImVec2(0.0f, textHeight * 11);
    if (ImGui::BeginTable("TimeProfilerTearDown", 3, flags, size)) {
        // Header
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("00:00:00.000").x);
        ImGui::TableSetupColumn("Total");
        ImGui::TableHeadersRow();

        // Content
        if (functionTime.size()) {
            ImGuiListClipper clipper;
            clipper.Begin(functionTime.size());
            while (clipper.Step()) {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++) {
                    ImGui::TableNextRow();

                    // Name
                    ImGui::TableNextColumn();
                    ImGui::Text(functionTime[row].second);

                    // Time
                    ImGui::TableNextColumn();
                    constexpr uint64_t nsToMs = 1000 * 1000;
                    constexpr uint64_t nsToS = 1000 * nsToMs;
                    constexpr uint64_t nsToM = 60 * nsToS;
                    constexpr uint64_t nsToH = 60 * nsToM;
                    const uint64_t time = functionTime[row].first;
                    int h = time / nsToH;
                    int m = (time % nsToH) / nsToM;
                    int s = (time % nsToM) / nsToS;
                    int ms = (time % nsToS) / nsToMs;
                    ImGui::Text("%02d:%02d:%02d.%03d", h, m, s, ms);

                    // Total
                    ImGui::TableNextColumn();
                    ImGui::Text("Noice");
                }
            }
        }

        ImGui::EndTable();
    }
}

} // namespace atta::ui
