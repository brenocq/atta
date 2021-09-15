//--------------------------------------------------
// Atta Graphics System
// menuBar.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/layers/internal/editor/menuBar.h>
#include <atta/eventSystem/events/windowCloseEvent.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/fileSystem/fileManager.h>
#include <imgui_internal.h>

namespace atta
{
	MenuBar::MenuBar():
		_showPreferences(false)
	{

	}

	void MenuBar::render()
	{
		if(ImGui::BeginMainMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{

				if(ImGui::BeginMenu("Project"))
				{
					if(FileManager::isProjectOpen())
					{
						if(ImGui::BeginMenu(FileManager::getProject()->getName().c_str()))
						{
							if(ImGui::MenuItem("Close"))
								FileManager::closeProject();

							ImGui::EndMenu();
						}
					}
					else
					{
						if(ImGui::MenuItem("Open"))
							_showOpenProject = true;
					}
					ImGui::EndMenu();
				}

				ImGui::Separator();

				if(ImGui::MenuItem("Quit"))
				{
					WindowCloseEvent e;
					EventManager::publish(e);
				}
				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Edit"))
			{
				if(ImGui::MenuItem("Preferences"))
					_showPreferences = true;

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
		openProjectModal();
		preferences();
	}

	void MenuBar::preferences()
	{
		if(_showPreferences)
		{
			if(ImGui::Begin("Preferences", &_showPreferences))
			{
				std::vector<std::string> stringIds = StringId::getStrings();
				for(const auto& str : stringIds)
					ImGui::Text(str.c_str());
			}
			ImGui::End();
		}
	}

	void MenuBar::openProjectModal()
	{
		static bool lastShow = false;
		if(_showOpenProject && !lastShow)
		{
			// OBS: Doing this because can't open popup inside menuitem
			ImGui::OpenPopup("Open Project##OpenProjectModal");
			lastShow = _showOpenProject;
		}

        if(ImGui::BeginPopupModal("Open Project##OpenProjectModal"))
        {
            ImGui::Text("For now, you need to paste the absolute location for the .atta file below");
            ImGui::Separator();

            static char buf[254] = "";
            ImGui::InputText("##openProjectAttaPath", buf, sizeof(buf));

            if(ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
				_showOpenProject = false;
				lastShow = false;
			}

            ImGui::SameLine();

            if(ImGui::Button("Open"))
			{ 
				FileManager::openProject(fs::path(buf));
				ImGui::CloseCurrentPopup();
				_showOpenProject = false;
				lastShow = false;
			}
            ImGui::SetItemDefaultFocus();

            ImGui::EndPopup();
        }
	}
}
