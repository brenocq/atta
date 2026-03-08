//--------------------------------------------------
// Simple Script
// cubeScript.cpp
// Date: 2021-11-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "cubeScript.h"
#include <atta/component/components/transform.h>

void CubeScript::update(cmp::Entity entity, float dt) {
    // Update time
    static float time = 0;
    time += dt * 1.2f;
    if (time > 2 * M_PI)
        time -= 2 * M_PI;

    // Get the entity transform component
    cmp::Transform* t = entity.get<cmp::Transform>();

    t->scale.x = 0.1f * std::sin(time) + 0.9f;
    t->scale.y = 0.1f * std::sin(time) + 0.9f;
    t->scale.z = 0.1f * std::sin(time) + 0.9f;
}
