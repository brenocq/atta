//--------------------------------------------------
// Atta UI System
// versionWindow.cpp
// Date: 2022-06-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/layers/editor/topBar/localWindows/versionWindow.h>
#include <atta/cmakeConfig.h>
#include <imgui_internal.h>

namespace atta::ui
{
	VersionWindow::VersionWindow()
	{
        setName("Version");
	}

	void VersionWindow::renderImpl()
	{
        static bool checkVersions = true;
        if(checkVersions)
        {
            checkVersions = false;
            checkInstalledVersions();
        }

		ImGui::Text(("atta " + std::string(ATTA_VERSION)).c_str());
		ImGui::Separator();
		ImGui::Text("Installed versions: ");
        for(std::string iv : _installedVersions)
        {
		    ImGui::Text(iv.c_str());
            ImGui::SameLine();

            if(ImGui::Button(("Open##Open"+iv).c_str()))
            {
                std::system((iv+"&").c_str());
                exit(0);
            }
        }

	}

    void VersionWindow::checkInstalledVersions()
    {
        for(const auto& entry : fs::directory_iterator(fs::path(ATTA_INSTALLED_VERSIONS_DIR)))
        {
            if(fs::is_directory(entry))
            {
                std::string full = entry.path().string();
                unsigned start = full.size()-1;
                while(start > 0 && full[start] != '/')
                    start--;
                std::string name = full.substr(start+1, name.size()-start-1);
                if(name.find("atta-") != std::string::npos)
                    _installedVersions.insert(name);
            }
        }
    }
}
