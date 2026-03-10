//--------------------------------------------------
// Physics 3D - Collision Check
// projectScript.cpp
// Date: 2022-10-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "projectScript.h"
#include <atta/component/components/material.h>
#include <atta/component/interface.h>
#include <atta/physics/interface.h>

const cmp::Entity cube0(0);
const cmp::Entity cube1(2);

void Project::onUpdateBefore(float delta) {
    if (phy::areColliding(cube0, cube1)) {
        cube0.get<cmp::Material>()->set("colliding");
        cube1.get<cmp::Material>()->set("colliding");
    } else {
        cube0.get<cmp::Material>()->set("not-colliding");
        cube1.get<cmp::Material>()->set("not-colliding");
    }
}
