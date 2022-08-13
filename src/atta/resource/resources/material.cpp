//--------------------------------------------------
// Atta Resource Module
// material.cpp
// Date: 2022-08-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/resource/resources/material.h>

namespace atta::resource {

Material::Material(const fs::path& filename) : Material(filename, {}) {}
Material::Material(const fs::path& filename, CreateInfo info) : Resource(filename) {
    albedo = info.albedo;
    metallic = info.metallic;
    roughness = info.roughness;
    ao = info.ao;
    albedoImage = info.albedoImage;
    metallicImage = info.metallicImage;
    roughnessImage = info.roughnessImage;
    aoImage = info.aoImage;
}

bool Material::albedoIsImage() const { return albedoImage != emptyImage; }
bool Material::metallicIsImage() const { return metallicImage != emptyImage; }
bool Material::roughnessIsImage() const { return roughnessImage != emptyImage; }
bool Material::aoIsImage() const { return aoImage != emptyImage; }
bool Material::hasNormalImage() const { return normalImage != emptyImage; }

void Material::setAlbedo(vec3f value) {
    albedo = value;
    resetAlbedoImage();
}
void Material::setAlbedo(StringId texture) { albedoImage = texture; }
void Material::setMetallic(float value) {
    metallic = value;
    resetMetallicImage();
}
void Material::setMetallic(StringId texture) { metallicImage = texture; }
void Material::setRoughness(float value) {
    roughness = value;
    resetRoughnessImage();
}
void Material::setRoughness(StringId texture) { roughnessImage = texture; }
void Material::setAo(float value) {
    ao = value;
    resetAoImage();
}
void Material::setAo(StringId texture) { aoImage = texture; }
void Material::setNormal(StringId texture) { normalImage = texture; }

void Material::resetAlbedoImage() { albedoImage = emptyImage; }
void Material::resetMetallicImage() { metallicImage = emptyImage; }
void Material::resetRoughnessImage() { roughnessImage = emptyImage; }
void Material::resetAoImage() { aoImage = emptyImage; }
void Material::resetNormalImage() { normalImage = emptyImage; }

} // namespace atta::resource
