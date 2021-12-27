//--------------------------------------------------
// Atta UI System
// repoWindow.cpp
// Date: 2021-12-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/layers/editor/topBar/localWindows/repoWindow.h>
#include <atta/fileSystem/fileManager.h>
#include <imgui.h>

namespace atta::ui
{
    RepoWindow::RepoWindow(fs::path repoPath):
        _repoPath(repoPath), _shouldClose(false)
    {
        _title = repoPath.filename();
        _description = "Repository without description";
        if(fs::exists(repoPath/"README.md"))
        {
            ReadmeMetadata meta = parseReadmeMetadata(repoPath/"README.md");
            if(meta.title.size())
                _title = meta.title;
            if(meta.description.size())
                _description = meta.description;
        }

        findProjects();
    }

    void RepoWindow::render()
    {
        bool open = !_shouldClose;
        if(ImGui::Begin((_title+"###RepoWindow"+_repoPath.string()).c_str(), &open))
        {
            ImGui::Text(_title.c_str());
            ImGui::Text(_description.c_str());
            ImGui::Separator();

            if(ImGui::BeginTable(("repoWindowTable###RepoWindowTable"+_repoPath.string()).c_str(), 2, ImGuiTableFlags_Borders))
            {
                ImGui::TableSetupColumn("description", ImGuiTableColumnFlags_WidthStretch, 300.0f);
                ImGui::TableSetupColumn("open", ImGuiTableColumnFlags_WidthFixed, 40.0f);
                
                for(ProjectInfo& project : _projects)
                {
                    // Project info
                    ImGui::TableNextColumn();
                    ImGui::Text(project.metadata.title.c_str());
                    ImGui::Spacing();
                    ImGui::Text(project.metadata.description.c_str());

                    // Open project button
                    ImGui::TableNextColumn();
                    if(ImGui::Button(("Open###RepoWindowOpenButton"+project.attaFile.string()).c_str()))
                        FileManager::openProject(project.attaFile);
                    ImGui::TableNextRow();
                }

                ImGui::EndTable();
            }
        }
        ImGui::End();
        _shouldClose = !open;
    }

    void RepoWindow::findProjects()
    {
        for(auto& entry : fs::recursive_directory_iterator(_repoPath))
        {
            fs::path p = entry.path();
            if(p.extension().string() == ".atta")
            {
                ProjectInfo pi;
                pi.attaFile = p;
                pi.metadata = parseReadmeMetadata(p.parent_path()/"README.md");
                _projects.push_back(pi);
            }
        }
    }

    RepoWindow::ReadmeMetadata RepoWindow::parseReadmeMetadata(fs::path readme)
    {
        ReadmeMetadata meta {};

        // Check if file is a README.md file
        if(readme.filename().string() != "README.md")
        {
            LOG_WARN("RepoWindow", "(parseReadmeMetadata) Expected readme file, but got $0", readme.filename().string());
            return meta;
        }

        // Read file
        std::ifstream readmeFile(readme);
        if(readmeFile.is_open())
        {
            std::stringstream buffer;
            buffer << readmeFile.rdbuf();
            std::string readmeStr = buffer.str();

            // Parse metadata
            if(readmeStr.substr(0,3) == "---")
            {
                int start = 4;
                while(true)
                {
                    size_t endMark = readmeStr.find(':', start);
                    size_t nl = readmeStr.find('\n', start);
                    size_t endMeta = readmeStr.find("---", start);
                    if(endMark == std::string::npos ||
                            nl == std::string::npos ||
                            endMeta == std::string::npos)// File not well formated
                        break;
                    if(endMeta < nl)// End of metadata
                        break;

                    std::string marker = readmeStr.substr(start, endMark-start);
                    std::string value = readmeStr.substr(endMark+1, nl-endMark-1l);
                    if(value[0] == ' ') value = value.substr(1, value.size()-1);

                    if(marker == "title")
                        meta.title = value;
                    else if(marker == "description")
                        meta.description = value;
                    else if(marker == "image")
                        meta.image = value;

                    start = nl+1;
                }
            }

            readmeFile.close();
        }
        else
        {
            // Could no open file, return
            LOG_WARN("RepoWindow", "(parseReadmeMetadata) Could not open $0", readme.string());
            return meta;
        }
        return meta;
    }
}
