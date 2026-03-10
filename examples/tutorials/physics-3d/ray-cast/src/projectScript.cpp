//--------------------------------------------------
// Physics 3D - Ray Cast
// projectScript.cpp
// Date: 2022-10-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "projectScript.h"
#include "imgui.h"
#include <atta/component/components/material.h>
#include <atta/component/components/relationship.h>
#include <atta/component/components/transform.h>
#include <atta/component/interface.h>
#include <atta/graphics/drawer.h>
#include <atta/graphics/interface.h>
#include <atta/physics/interface.h>

const cmp::Entity ctrl0(6);
const cmp::Entity ctrl1(7);
const cmp::Entity cubes(0);

void Project::onUpdateBefore(float delta) {
    // Reset materials
    for (cmp::Entity cube : cubes.get<cmp::Relationship>()->getChildren())
        cube.get<cmp::Material>()->set("no-hit");

    // Set hit materials
    atta::vec3 begin = ctrl0.get<cmp::Transform>()->position;
    atta::vec3 end = ctrl1.get<cmp::Transform>()->position;
    std::vector<phy::RayCastHit> hits = phy::rayCast(begin, end, _onlyFirst);
    for (const auto& hit : hits)
        hit.entity.get<cmp::Material>()->set("hit");
}

void Project::onAttaLoop() {
    // Clear old line between control spheres
    gfx::Drawer::clear("control");

    // Create line between control spheres
    gfx::Drawer::Line line;
    line.p0 = ctrl0.get<cmp::Transform>()->position;
    line.p1 = ctrl1.get<cmp::Transform>()->position;
    line.c0 = {1, 0, 0, 1};
    line.c1 = {1, 0, 0, 1};
    gfx::Drawer::add(line, "control");
}

void Project::onUIRender() {
    ImGui::SetNextWindowSize({150, 80}, ImGuiCond_Once);
    ImGui::Begin("Ray cast control");
    ImGui::Checkbox("Only first", &_onlyFirst);
    ImGui::End();
}
