//--------------------------------------------------
// Atta Resource Module
// material.cpp
// Date: 2022-08-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/serializer/serializer.h>
#include <atta/resource/resources/material.h>

namespace atta::resource {

Material::Material() : Material(fs::path()) {}
Material::Material(const fs::path& filename) : Material(filename, {}) {}
Material::Material(const fs::path& filename, CreateInfo info) : Resource(filename) {
    color = info.color;
    metallic = info.metallic;
    roughness = info.roughness;
    ao = info.ao;
    colorImage = info.colorImage;
    metallicImage = info.metallicImage;
    roughnessImage = info.roughnessImage;
    aoImage = info.aoImage;
}

bool Material::colorIsImage() const { return colorImage != emptyImage; }
bool Material::metallicIsImage() const { return metallicImage != emptyImage; }
bool Material::roughnessIsImage() const { return roughnessImage != emptyImage; }
bool Material::aoIsImage() const { return aoImage != emptyImage; }
bool Material::hasNormalImage() const { return normalImage != emptyImage; }

void Material::setColor(vec3f value) {
    color = value;
    colorImage = {};
}
void Material::setColor(StringId texture) { colorImage = texture; }
void Material::setMetallic(float value) {
    metallic = value;
    metallicImage = {};
}
void Material::setMetallic(StringId texture) { metallicImage = texture; }
void Material::setRoughness(float value) {
    roughness = value;
    roughnessImage = {};
}
void Material::setRoughness(StringId texture) { roughnessImage = texture; }
void Material::setAo(float value) {
    ao = value;
    aoImage = {};
}
void Material::setAo(StringId texture) { aoImage = texture; }
void Material::setNormal(StringId texture) { normalImage = texture; }

void Material::serialize(std::ostream& os) {
    file::write(os, _id);
    file::write(os, color);
    file::write(os, metallic);
    file::write(os, roughness);
    file::write(os, ao);
    file::write(os, colorImage);
    file::write(os, metallicImage);
    file::write(os, roughnessImage);
    file::write(os, aoImage);
    file::write(os, normalImage);
    LOG_DEBUG("res::Material", "Values $0 $1 $2 $3 $4", colorImage, metallicImage, roughnessImage, aoImage, normalImage);
}

void Material::deserialize(std::istream& is) {
    file::read(is, _id);
    file::read(is, color);
    file::read(is, metallic);
    file::read(is, roughness);
    file::read(is, ao);
    file::read(is, colorImage);
    file::read(is, metallicImage);
    file::read(is, roughnessImage);
    file::read(is, aoImage);
    file::read(is, normalImage);
}

unsigned Material::getSerializedSize() { return file::Serializable::getSerializedSize(this); }

} // namespace atta::resource
