// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/component/interface.h>
#include <atta/resource/resources/material.h>

namespace atta::component {

/// %Component to define entity rendering material
/** When the texture is not defined, the same value is
 * used to the whole mesh. If the texture is defined, the attribute can
 * vary along the mesh.
 *
 * The entity must have Transform and Mesh to be rendered.
 */
struct Material final : public Component {
    StringId sid = "Material"; ///< Material name

    resource::Material* getResource() const;
    void setResource(const resource::Material* material);

    void set(StringId sid);
    void set(const resource::Material* material);
};
ATTA_REGISTER_COMPONENT(Material)
template <>
ComponentDescription& TypedComponentRegistry<Material>::getDescription();

} // namespace atta::component
