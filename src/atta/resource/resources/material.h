//--------------------------------------------------
// Atta Resource Module
// material.h
// Date: 2022-08-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RESOURCE_RESOURCES_MATERIAL_H
#define ATTA_RESOURCE_RESOURCES_MATERIAL_H

#include <atta/memory/allocatedObject.h>
#include <atta/resource/resource.h>
#include <atta/file/serializer/serializable.h>

namespace atta::resource {

class Material : public Resource, public memory::AllocatedObject<Material, SID("ResourceAllocator")>, public file::Serializable {
  public:
      static constexpr StringHash emptyImage = "emptyStringId"_sid;

    struct CreateInfo {
        vec3f color = vec3(1.0f, 0.0f, 1.0f);
        float metallic = 0.0f;
        float roughness = 0.5f;
        float ao = 1.0f;

        StringId colorImage = emptyImage;
        StringId metallicImage = emptyImage;
        StringId roughnessImage = emptyImage;
        StringId aoImage = emptyImage;
        StringId normalImage = emptyImage;
    };

    Material();
    Material(const fs::path& filename);
    Material(const fs::path& filename, CreateInfo info);

    bool colorIsImage() const;
    bool metallicIsImage() const;
    bool roughnessIsImage() const;
    bool aoIsImage() const;
    bool hasNormalImage() const;

    void setColor(vec3f value);
    void setColor(StringId texture);
    void setMetallic(float value);
    void setMetallic(StringId texture);
    void setRoughness(float value);
    void setRoughness(StringId texture);
    void setAo(float value);
    void setAo(StringId texture);
    void setNormal(StringId texture);

    // Serializable
    void serialize(std::ostream& os) override;
    void deserialize(std::istream& is) override;
    unsigned getSerializedSize();

    vec3f color;    ///< Material color
    float metallic;  ///< Metallic coefficient. 0->dielectric, 1->metallic
    float roughness; ///< Roughness coefficient. 0->very smooth, 1->very rough
    float ao;        ///< Ambient occlusion coefficient. 0->totally occluded, 1->not occluded

    StringId colorImage;    ///< Override with texture
    StringId metallicImage;  ///< Override Material::metallic with texture
    StringId roughnessImage; ///< Override Material::roughness with texture
    StringId aoImage;        ///< Override Material::ao with texture
    StringId normalImage;    ///< Material normal map
};

} // namespace atta::resource

#endif // ATTA_RESOURCE_RESOURCES_MATERIAL_H
