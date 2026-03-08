// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/cmakeConfig.h>
#include <atta/file/interface.h>
#include <atta/ui/panels/topBar/localWindows/examplesWindow.h>
#include <fstream>
#include <imgui.h>
#include <regex>
#include <sstream>

namespace atta::ui {

ExamplesWindow::ExamplesWindow() {
    setName("Examples");
    _initialSize = vec2(600.0f, 500.0f);
}

void ExamplesWindow::renderImpl() {
    // Scan once when window opens
    if (_examples.empty())
        scanExamples();

    if (_examples.empty()) {
        ImGui::TextDisabled("No examples found in %s/examples/", ATTA_DIR);
        return;
    }

    ImGui::TextDisabled("%zu example(s) found", _examples.size());
    ImGui::Separator();

    for (const ExampleInfo& ex : _examples) {
        ImGui::PushID(ex.attaFile.string().c_str());

        ImGui::TextUnformatted(ex.title.c_str());
        if (!ex.description.empty()) {
            ImGui::SameLine();
            ImGui::TextDisabled(" - %s", ex.description.c_str());
        }

        ImGui::SameLine(ImGui::GetContentRegionAvail().x - 40.0f);
        if (ImGui::Button("Open")) {
            file::openProject(ex.attaFile);
            setOpen(false);
        }

        ImGui::Separator();
        ImGui::PopID();
    }
}

void ExamplesWindow::scanExamples() {
    fs::path examplesDir = fs::path(ATTA_DIR) / "examples";
    if (!fs::exists(examplesDir) || !fs::is_directory(examplesDir))
        return;

    // Recursively find directories that contain a .atta file
    for (const auto& entry : fs::recursive_directory_iterator(examplesDir, fs::directory_options::skip_permission_denied)) {
        if (!fs::is_regular_file(entry) || entry.path().extension() != ".atta")
            continue;

        ExampleInfo info;
        info.attaFile = entry.path();

        // Use filename stem as fallback title
        info.title = entry.path().stem().string();

        // Try to parse README.md in the same directory
        fs::path readme = entry.path().parent_path() / "README.md";
        parseReadme(readme, info.title, info.description);

        _examples.push_back(std::move(info));
    }
}

bool ExamplesWindow::parseReadme(const fs::path& readme, std::string& title, std::string& description) {
    std::ifstream file(readme);
    if (!file.is_open())
        return false;

    // Read until we find <!-- ... --> block
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::regex commentBlock(R"(<!--([\s\S]*?)-->)");
    std::smatch match;
    if (!std::regex_search(content, match, commentBlock))
        return false;

    std::string block = match[1].str();

    // Extract title
    std::regex titleRe(R"(title:\s*(.+))");
    std::smatch titleMatch;
    if (std::regex_search(block, titleMatch, titleRe))
        title = titleMatch[1].str();
    // Trim trailing whitespace
    title.erase(title.find_last_not_of(" \t\r\n") + 1);

    // Extract description
    std::regex descRe(R"(description:\s*(.+))");
    std::smatch descMatch;
    if (std::regex_search(block, descMatch, descRe))
        description = descMatch[1].str();
    description.erase(description.find_last_not_of(" \t\r\n") + 1);

    return true;
}

} // namespace atta::ui
