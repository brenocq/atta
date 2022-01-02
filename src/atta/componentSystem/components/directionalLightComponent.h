//--------------------------------------------------
// Atta Component System
// directionalLightComponent.h
// Date: 2021-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_DIRECTIONAL_LIGHT_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_DIRECTIONAL_LIGHT_COMPONENT_H
#include <atta/componentSystem/componentManager.h>

namespace atta
{
	/// %Component to create directional light
	/** The light direction is the entity Z axis, you can change the transform rotation to change the light direction.
	 *
	 * A TransformComponent is needed to the light to be rendered.
	 */
    struct DirectionalLightComponent final : public Component
    {
        vec3f intensity = vec3f(1.0f, 1.0f, 1.0f);
    };
    ATTA_REGISTER_COMPONENT(DirectionalLightComponent)
    template<> ComponentDescription& TypedComponentRegistry<DirectionalLightComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_DIRECTIONAL_LIGHT_COMPONENT_H
