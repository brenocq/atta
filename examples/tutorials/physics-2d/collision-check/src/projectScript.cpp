//--------------------------------------------------
// Physics 2D - Collision Check
// projectScript.cpp
// Date: 2022-10-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "projectScript.h"
#include <atta/component/components/material.h>
#include <atta/component/interface.h>
#include <atta/physics/interface.h>

const cmp::Entity ground(0);
const cmp::Entity box(1);

void Project::onStop() {
    ground.get<cmp::Material>()->set("not-colliding");
    box.get<cmp::Material>()->set("not-colliding");
}

void Project::onUpdateBefore(float delta) {
    if (phy::areColliding(ground, box)) {
        ground.get<cmp::Material>()->set("colliding");
        box.get<cmp::Material>()->set("colliding");
    } else {
        ground.get<cmp::Material>()->set("not-colliding");
        box.get<cmp::Material>()->set("not-colliding");
    }
}
