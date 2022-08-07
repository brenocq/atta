//--------------------------------------------------
// Atta UI Module
// physicsSystemWindow.cpp
// Date: 2021-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/physics/manager.h>
#include <atta/ui/layers/editor/systemWindows/physicsSystemWindow.h>

namespace atta::ui {

PhysicsSystemWindow::PhysicsSystemWindow() { setName("Physics Module"); }

void PhysicsSystemWindow::renderImpl() {
    ImGui::Text("Physics Engine");
    std::vector<std::string> physicsEngines = {"Null", "Box2D", "Bullet"};

    physics::PhysicsEngine::Type selected = physics::Manager::getSelectedEngine();
    if (ImGui::BeginCombo(("##" + _name + "SelectEngine").c_str(), physicsEngines[selected].c_str())) {
        for (unsigned i = 0; i < physicsEngines.size(); i++) {
            if (ImGui::Selectable(physicsEngines[i].c_str(), i == selected))
                physics::Manager::setSelectedEngine((physics::PhysicsEngine::Type)i);
            if (i == selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();
}

} // namespace atta::ui
