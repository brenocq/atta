//--------------------------------------------------
// Atta UI System
// fileSelectionWindow.cpp
// Date: 2022-01-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/layers/editor/windows/utils/fileSelectionWindow.h>

namespace atta::ui
{
    FileSelectionWindow::FileSelectionWindow()
    {
        setName("File Selection");
        setPathImpl(fs::current_path());
    }

    void FileSelectionWindow::setPath(fs::path currPath) { getInstance().setPathImpl(currPath); }
    void FileSelectionWindow::setPathImpl(fs::path currPath)
    {
        // Copy new path string to buffer
        std::string newPathString = fs::absolute(currPath).string();
        unsigned i = 0;
        for(; i < std::min(newPathString.size(), _currPathBuffer.size()); i++)
            _currPathBuffer[i] = newPathString[i];
        _currPathBuffer[std::min(i, unsigned(_currPathBuffer.size()-1))] = '\0';

        _currPath = currPath;
    }

    fs::path FileSelectionWindow::getChosenFile()
    {
        return getInstance()._chosenFile;
    }

    void FileSelectionWindow::renderImpl()
    {
        // Ensure chosen file is empty until the file is actually chosen
        _chosenFile = fs::path();

        // Back button
        if(ImGui::Button("Back###FileSelectionWindowBack"))
            setPath(_currPath.parent_path());
        ImGui::SameLine();
        // Show current path
        if(ImGui::InputText("Current path###FileSelectionWindowCurrentPath", 
                    _currPathBuffer.data(), _currPathBuffer.size(), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            fs::path testNewPath(_currPathBuffer.data());
            if(fs::exists(testNewPath))
                setPath(testNewPath);
            else
            {
                LOG_WARN("FileSelectionWindow", "Trying to change to invalid path [w]$0[]", _currPathBuffer.data());
                setPath(_currPath);
            }
        }

        //----- Show files -----//
        ImGui::Separator();
        renderFileTree();

        //----- Bottom buttons -----//
        ImGui::Separator();

        if(ImGui::Button("Cancel###FileSelectionWindowCancel"))
            setOpen(false);
        ImGui::SameLine();
        if(ImGui::Button("Choose###FileSelectionWindowChoose"))
        {
            _chosenFile = _selectedFile;
            setOpen(false);
        }
    }

    template <typename TP>
    std::time_t to_time_t(TP tp)
    {
        using namespace std::chrono;
        auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
                  + system_clock::now());
        return system_clock::to_time_t(sctp);
    }

    void FileSelectionWindow::renderFileTree()
    {
        // Populate files and directories
        std::vector<fs::path> directories;
        std::vector<fs::path> files;
        for(auto& entry : fs::directory_iterator(_currPath))
            if(entry.is_directory())
                directories.push_back(entry.path());
            else
                files.push_back(entry.path());

        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
        static ImGuiSelectableFlags selectableFlags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowItemOverlap;
        static int selectedItem = 0;

        ImVec2 region = ImGui::GetContentRegionAvail();
        if(ImGui::BeginTable("filetree", 3, flags, ImVec2(region.x, region.y-30.0f)))
        {
            ImGui::TableSetupScrollFreeze(0, 1);// Top row always visible
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 50.0f);
            ImGui::TableSetupColumn("Date modified", ImGuiTableColumnFlags_WidthFixed, 80.0f);
            ImGui::TableHeadersRow();

            // Clipper to deal with folders with a lot of files
            ImGuiListClipper clipper;
            clipper.Begin(directories.size()+files.size());
            while(clipper.Step())
            {
                for(int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                {
                    fs::path rowPath = unsigned(row) < directories.size() ? directories[row] : files[row-directories.size()];

                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    if(unsigned(row) < directories.size())
                    {
                        // Directory logic
                        if(ImGui::Selectable(directories[row].filename().string().c_str(), selectedItem == row, selectableFlags))
                        {
                            if(selectedItem == row)
                            {
                                setPath(directories[row]);
                                selectedItem = 0;
                            }
                            else 
                                selectedItem = row;
                        }
                    }
                    else
                    {
                        // File logic
                        if(ImGui::Selectable(rowPath.filename().string().c_str(), selectedItem == row, selectableFlags))
                        {
                            if(selectedItem == row)
                            {
                                _chosenFile = _selectedFile;
                                setOpen(false);
                            }
                            else
                            {
                                selectedItem = row;
                                _selectedFile = rowPath;
                            }
                        }

                        ImGui::TableSetColumnIndex(1);
                        // Show human readable file size
                        int i = 0;
                        double mantissa = fs::file_size(rowPath);
                        for(; mantissa >= 1024.; mantissa /= 1024., i++){}
                        mantissa = std::ceil(mantissa * 10.) / 10.;
                        const char* sizes[7] = {"B","KB","MB","GB","TB","PB","EB"};
                        ImGui::Text("%d %s", int(ceil(mantissa)), sizes[i]);
                    }

                    // Show human readable last write
                    ImGui::TableSetColumnIndex(2);
                    auto ftime = std::filesystem::last_write_time(rowPath);
                    std::time_t cftime = to_time_t(ftime);
                    char buf[64];
                    strftime(buf, sizeof buf, "%e %b %Y %H:%M", std::localtime(&cftime));
                    ImGui::Text("%s", buf);
                }
            }
            ImGui::EndTable();
        }
    }
}
