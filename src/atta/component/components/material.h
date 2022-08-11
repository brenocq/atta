//--------------------------------------------------
// Atta Component Module
// material.h
// Date: 2021-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_MATERIAL_H
#define ATTA_COMPONENT_COMPONENTS_MATERIAL_H

#include <atta/component/interface.h>

namespace atta::component {

/// %Component to define entity rendering material
/** When the texture is not defined, the same value is
 * used to the whole mesh. If the texture is defined, the attribute can
 * vary along the mesh.
 *
 * The entity must have Transform and Mesh to be rendered.
 */
struct Material final : public Component {
    vec3f albedo = vec3(1.0f, 0.0f, 1.0f); ///< Material albedo
    float metallic = 0.0f;                 ///< Metallic coefficient. 0->dielectric, 1->metal
    float roughness = 0.5f;                ///< Roughness coefficient. 0->very smooth, 1->very rough
    float ao = 1.0f;                       ///< Ambient occlusion coefficient. 0->totally occluded, 1->not occluded

    StringId albedoTexture = StringId("Empty texture");    ///< Override with texture
    StringId metallicTexture = StringId("Empty texture");  ///< Override Material::metallic with texture
    StringId roughnessTexture = StringId("Empty texture"); ///< Override Material::roughness with texture
    StringId aoTexture = StringId("Empty texture");        ///< Override Material::ao with texture
    StringId normalTexture = StringId("Empty texture");    ///< Material normal map
};
ATTA_REGISTER_COMPONENT(Material)
template <>
ComponentDescription& TypedComponentRegistry<Material>::getDescription();

} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_MATERIAL_H
