//--------------------------------------------------
// Simple Script
// lightScript.cpp
// Date: 2021-11-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "lightScript.h"
#include <atta/component/components/transform.h>

void LightScript::update(cmp::Entity entity, float dt) {
    // Update time
    static float time = 0;
    if (entity.getId() == 1)
        time += dt * 0.3f;
    if (time > 2 * M_PI)
        time -= 2 * M_PI;

    // Get the entity transform component
    cmp::Transform* t = entity.get<cmp::Transform>();

    // Rotate the point lights using the entityId as offset
    t->position.x = 1.2f * std::cos(time + (float)entity.getId() * 2);
    t->position.y = 1.2f * std::sin(time + (float)entity.getId() * 2);
    t->position.z = 1.2f;
}
