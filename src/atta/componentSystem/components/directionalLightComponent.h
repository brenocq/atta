//--------------------------------------------------
// Atta Component System
// directionalLightComponent.h
// Date: 2021-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_DIRECTIONAL_LIGHT_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_DIRECTIONAL_LIGHT_COMPONENT_H
#include <atta/core/math/math.h>

namespace atta
{
    struct DirectionalLightComponent
    {
        vec3 intensity = vec3(1.0f, 1.0f, 1.0f);
    };
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_DIRECTIONAL_LIGHT_COMPONENT_H
