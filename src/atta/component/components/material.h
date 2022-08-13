//--------------------------------------------------
// Atta Component Module
// material.h
// Date: 2021-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_MATERIAL_H
#define ATTA_COMPONENT_COMPONENTS_MATERIAL_H

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
    void set(StringId sid);
    void set(const resource::Material* material);
};
ATTA_REGISTER_COMPONENT(Material)
template <>
ComponentDescription& TypedComponentRegistry<Material>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_MATERIAL_H
