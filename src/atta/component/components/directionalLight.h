// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

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
ComponentDescription& TypedComponentRegistry<DirectionalLight>::getDescription();

} // namespace atta::component
