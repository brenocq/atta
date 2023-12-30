//--------------------------------------------------
// Atta Resource Module
// material.h
// Date: 2022-08-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RESOURCE_RESOURCES_MATERIAL_H
#define ATTA_RESOURCE_RESOURCES_MATERIAL_H

#include <atta/file/serializer/serializable.h>
#include <atta/memory/allocatedObject.h>
#include <atta/resource/resource.h>

namespace atta::resource {

class Manager;
class Material : public Resource, public memory::AllocatedObject<Material, SID("ResourceAllocator")>, public file::Serializable {
  public:
    struct CreateInfo {
        vec3f color = vec3(1.0f, 0.0f, 1.0f);
        float metallic = 0.0f;
        float roughness = 0.5f;
        float ao = 1.0f;

        StringId colorImage = {};
        StringId metallicImage = {};
        StringId roughnessImage = {};
        StringId aoImage = {};
        StringId normalImage = {};
    };

    Material();
    Material(const fs::path& filename);
    Material(const fs::path& filename, CreateInfo info);

    bool colorIsImage() const;
    bool metallicIsImage() const;
    bool roughnessIsImage() const;
    bool aoIsImage() const;
    bool hasNormalImage() const;

    vec3f getColor() const;
    StringId getColorImage() const;
    float getMetallic() const;
    StringId getMetallicImage() const;
    float getRoughness() const;
    StringId getRoughnessImage() const;
    float getAo() const;
    StringId getAoImage() const;
    StringId getNormalImage() const;

    void setColor(vec3f value);
    void setColorImage(StringId texture);
    void setMetallic(float value);
    void setMetallicImage(StringId texture);
    void setRoughness(float value);
    void setRoughnessImage(StringId texture);
    void setAo(float value);
    void setAoImage(StringId texture);
    void setNormalImage(StringId texture);

    // Serializable
    void serialize(std::ostream& os) override;
    void deserialize(std::istream& is) override;
    unsigned getSerializedSize();

  private:
    /**
     * @brief Check if material was updated
     *
     * This method should only be called by the resource manager, it will be used to trigger material update events
     */
    bool wasUpdated();
    friend Manager;

    vec3f _color;     ///< Material color
    float _metallic;  ///< Metallic coefficient. 0->dielectric, 1->metallic
    float _roughness; ///< Roughness coefficient. 0->very smooth, 1->very rough
    float _ao;        ///< Ambient occlusion coefficient. 0->totally occluded, 1->not occluded

    StringId _colorImage;     ///< Override with texture
    StringId _metallicImage;  ///< Override Material::metallic with texture
    StringId _roughnessImage; ///< Override Material::roughness with texture
    StringId _aoImage;        ///< Override Material::ao with texture
    StringId _normalImage;    ///< Material normal map

    bool _updated; ///< Keep track if material was updated to trigger events
};

} // namespace atta::resource

#endif // ATTA_RESOURCE_RESOURCES_MATERIAL_H
