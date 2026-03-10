// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

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
