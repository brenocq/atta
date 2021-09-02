//--------------------------------------------------
// Atta Graphics System
// menuBar.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/layers/internal/editor/menuBar.h>
#include <atta/eventSystem/events/windowCloseEvent.h>
#include <atta/eventSystem/eventManager.h>
#include <imgui_internal.h>

namespace atta
{
	void MenuBar::render()
	{
		if(ImGui::BeginMainMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("Quit"))
				{
					WindowCloseEvent e;
					EventManager::publish(e);
				}
				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Edit"))
			{
				//if(ImGui::MenuItem("Undo", "CTRL+Z")) {}
				//if(ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				//ImGui::Separator();
				//if(ImGui::MenuItem("Cut", "CTRL+X")) {}
				//if(ImGui::MenuItem("Copy", "CTRL+C")) {}
				//if(ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}
}
