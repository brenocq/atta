//--------------------------------------------------
// Simple Script
// cubeScript.cpp
// Date: 2021-11-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "cubeScript.h"
#include <atta/component/components/material.h>
#include <atta/component/components/name.h>
#include <atta/component/components/transform.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/material.h>

void CubeScript::update(cmp::Entity entity, float dt) {
    // The static variables are shared among all CubeScripts
    static const cmp::Entity prototype(1);
    static unsigned numClones = 0;
    static float time = 0;

    cmp::Factory* factory = cmp::getFactory(prototype);

    int cloneId = entity.getId() - factory->getFirstClone().getId();

    //----- Get number of clones -----//
    if (numClones == 0)
        numClones = factory->getMaxClones();

    //----- Advance time -----//
    // Use clone 0 to advance the time
    if (cloneId == 0) {
        time += dt;
        if (time > 2 * M_PI)
            time -= 2 * M_PI;
    }

    //----- Transform Component -----//
    unsigned row = cloneId % (unsigned)std::sqrt(numClones);
    unsigned col = cloneId / (unsigned)std::sqrt(numClones);
    float offset = 8 * M_PI / numClones * (col + row) + time;

    // Update clone transform
    cmp::Transform* t = entity.get<cmp::Transform>();
    t->position.x = row;
    t->position.y = col;
    t->position.z = 1.0f + std::sin(offset);

    //----- Material Component -----//
    // Set material resource
    std::string materialName = "Material " + std::to_string(cloneId);
    res::Material* m = res::get<res::Material>(materialName);
    entity.get<cmp::Material>()->set(m);

    // Update material color
    atta::vec3f color;
    color.x = t->position.z * 0.5f;
    color.y = 1.0f - m->getColor().x;
    color.z = 1.0f;
    m->setColor(color);

    //----- Name Component -----//
    entity.get<cmp::Name>()->set("Cube clone " + std::to_string(cloneId));
}
