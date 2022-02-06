//--------------------------------------------------
// Atta UI System
// physicsSystemWindow.cpp
// Date: 2021-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/layers/editor/systemWindows/physicsSystemWindow.h>
#include <atta/physicsSystem/physicsManager.h>

namespace atta::ui
{
	PhysicsSystemWindow::PhysicsSystemWindow()
	{
        setName("Physics System");
	}

	void PhysicsSystemWindow::renderImpl()
	{
		ImGui::Text("Physics Engine");
		std::vector<std::string> physicsEngines = { "Null", "Box2D", "Bullet" };

		PhysicsEngine::Type selected = PhysicsManager::getSelectedEngine();
		if(ImGui::BeginCombo(("##"+_name+"SelectEngine").c_str(), physicsEngines[selected].c_str()))
		{
			for(unsigned i = 0; i < physicsEngines.size(); i++)
			{
				if(ImGui::Selectable(physicsEngines[i].c_str(), i == selected))
					PhysicsManager::setSelectedEngine((PhysicsEngine::Type)i);
				if(i == selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		ImGui::Separator();
	}
}
