//--------------------------------------------------
// Atta Component Module
// pointLight.h
// Date: 2021-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_POINT_LIGHT_H
#define ATTA_COMPONENT_COMPONENTS_POINT_LIGHT_H

#include <atta/component/interface.h>

namespace atta::component {

/// %Component to create point light
/** The light position is the entity position, you can change the
 * transform position to change the light position.
 *
 * A Transform is needed to the light to be rendered.
 */
struct PointLight final : public Component {
    vec3f intensity = vec3(1.0f, 1.0f, 1.0f);
};
ATTA_REGISTER_COMPONENT(PointLight)
template <>
ComponentDescription& TypedComponentRegistry<PointLight>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_POINT_LIGHT_H
