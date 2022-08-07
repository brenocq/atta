//--------------------------------------------------
// Atta Component Module
// pointLightComponent.h
// Date: 2021-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_POINT_LIGHT_COMPONENT_H
#define ATTA_COMPONENT_COMPONENTS_POINT_LIGHT_COMPONENT_H
#include <atta/component/componentManager.h>

namespace atta
{
	/// %Component to create point light
	/** The light position is the entity position, you can change the 
	 * transform position to change the light position.
	 *
	 * A TransformComponent is needed to the light to be rendered.
	 */
    struct PointLightComponent final : public Component
    {
        vec3f intensity = vec3(1.0f, 1.0f, 1.0f);
    };
    ATTA_REGISTER_COMPONENT(PointLightComponent)
    template<> ComponentDescription& TypedComponentRegistry<PointLightComponent>::getDescription();
}

#endif// ATTA_COMPONENT_COMPONENTS_POINT_LIGHT_COMPONENT_H
