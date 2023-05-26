//--------------------------------------------------
// Atta Component Module
// directionalLight.h
// Date: 2021-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_DIRECTIONAL_LIGHT_H
#define ATTA_COMPONENT_COMPONENTS_DIRECTIONAL_LIGHT_H

#include <atta/component/interface.h>

namespace atta::component {

/// %Component to create directional light
/** The light direction is the entity Z axis, you can change the transform rotation to change the light direction.
 *
 * A Transform is needed to the light to be rendered.
 */
struct DirectionalLight final : public Component {
    vec3f intensity = vec3f(1.0f, 1.0f, 1.0f);
};
ATTA_REGISTER_COMPONENT(DirectionalLight)
template <>
ComponentDescription& TypedRegistry<DirectionalLight>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_DIRECTIONAL_LIGHT_H
