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
					if(FileManager::isProjectDefined())
					{
						if(ImGui::BeginMenu(FileManager::getProjectName().c_str()))
						{
							if(ImGui::MenuItem("Close"))
								FileManager::closeProject();

							ImGui::EndMenu();
						}
					}
					else
						if(ImGui::MenuItem("Open"));
					ImGui::EndMenu();
				}
				else
				{
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
}
