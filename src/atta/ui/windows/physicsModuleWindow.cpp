//--------------------------------------------------
// Atta UI Module
// physicsModuleWindow.cpp
// Date: 2021-12-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/windows/physicsModuleWindow.h>

#include <atta/physics/engines/bulletEngine.h>
#include <atta/physics/interface.h>

namespace atta::ui {

PhysicsModuleWindow::PhysicsModuleWindow() { initialize("Physics Module"); }

void PhysicsModuleWindow::renderImpl() {
    ImGui::Text("Engine");

    ImGui::SameLine();
    std::vector<std::string> physicsEngines = {"None", "Box2D", "Bullet"};
    physics::Engine::Type selected = physics::getEngineType();
    if (ImGui::BeginCombo(("##" + _name + "SelectEngine").c_str(), physicsEngines[selected].c_str())) {
        for (unsigned i = 0; i < physicsEngines.size(); i++) {
            if (ImGui::Selectable(physicsEngines[i].c_str(), i == selected)) {
                selected = (physics::Engine::Type)i;
                physics::setEngineType(selected);
            }
            if (i == selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();
    switch (selected) {
        case physics::Engine::NONE:
            break;
        case physics::Engine::BOX2D: {
            bool showColliders = physics::getShowColliders();
            bool showContacts = physics::getShowContacts();
            bool showJoints = physics::getShowJoints();
            if (ImGui::Checkbox("Show colliders", &showColliders))
                physics::setShowColliders(showColliders);
            if (ImGui::Checkbox("Show contacts", &showContacts))
                physics::setShowContacts(showContacts);
            if (ImGui::Checkbox("Show joints", &showJoints))
                physics::setShowJoints(showJoints);

            // Gravity vector
            {
                ImGui::Text("Gravity");
                vec3 g = physics::getGravity();
                if (ImGui::DragFloat2("Gravity", (float*)(&g), 0.01f))
                    physics::setGravity(g);
            }
            break;
        }
        case physics::Engine::BULLET: {
            std::shared_ptr<physics::BulletEngine> bullet = std::static_pointer_cast<physics::BulletEngine>(physics::getEngine());

            bool showColliders = physics::getShowColliders();
            bool showAabb = bullet->getShowAabb();
            bool showContacts = physics::getShowContacts();
            bool showJoints = physics::getShowJoints();
            if (ImGui::Checkbox("Show colliders", &showColliders))
                physics::setShowColliders(showColliders);
            if (ImGui::Checkbox("Show aabb", &showAabb))
                bullet->setShowAabb(showAabb);
            if (ImGui::Checkbox("Show contacts", &showContacts))
                physics::setShowContacts(showContacts);
            if (ImGui::Checkbox("Show joints", &showJoints))
                physics::setShowJoints(showJoints);

            // Gravity vector
            {
                ImGui::Text("Gravity");
                vec3 g = physics::getGravity();
                if (ImGui::DragFloat3("Gravity", (float*)(&g), 0.01f))
                    physics::setGravity(g);
            }
            // Sub steps
            {
                int numSubSteps = bullet->getNumSubSteps();
                if (ImGui::DragInt("Sub steps", &numSubSteps, 1, 1, 1000)) {
                    if (numSubSteps < 1)
                        numSubSteps = 1;
                    bullet->setNumSubSteps(numSubSteps);
                }
            }
            ImGui::Separator();

            // Number of collisions
            unsigned numCollisions2 = 0;
            auto collisions = bullet->getCollisions();
            for (auto [bodyA, bodiesB] : collisions)
                numCollisions2 += bodiesB.size();
            ImGui::Text("Num collisions: %u", numCollisions2 / 2);
            break;
        }
    }
}

} // namespace atta::ui
