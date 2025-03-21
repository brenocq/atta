//--------------------------------------------------
// Atta Resource Module
// material.cpp
// Date: 2022-08-13
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/resource/resources/material.h>

namespace atta::resource {

Material::Material() : Material(fs::path()) {}
Material::Material(const fs::path& filename) : Material(filename, {}) {}
Material::Material(const fs::path& filename, CreateInfo info) : Resource(filename) {
    _color = info.color;
    _metallic = info.metallic;
    _roughness = info.roughness;
    _ao = info.ao;
    _colorImage = info.colorImage;
    _metallicImage = info.metallicImage;
    _roughnessImage = info.roughnessImage;
    _aoImage = info.aoImage;
}

bool Material::colorIsImage() const { return _colorImage != StringId(); }
bool Material::metallicIsImage() const { return _metallicImage != StringId(); }
bool Material::roughnessIsImage() const { return _roughnessImage != StringId(); }
bool Material::aoIsImage() const { return _aoImage != StringId(); }
bool Material::hasNormalImage() const { return _normalImage != StringId(); }

vec3f Material::getColor() const { return _color; }
StringId Material::getColorImage() const { return _colorImage; }
float Material::getMetallic() const { return _metallic; }
StringId Material::getMetallicImage() const { return _metallicImage; }
float Material::getRoughness() const { return _roughness; }
StringId Material::getRoughnessImage() const { return _roughnessImage; }
float Material::getAo() const { return _ao; }
StringId Material::getAoImage() const { return _aoImage; }
StringId Material::getNormalImage() const { return _normalImage; }

void Material::setColor(vec3f value) {
    _color = value;
    _colorImage = {};
    _updated = true;
}
void Material::setColorImage(StringId texture) {
    _colorImage = texture;
    _updated = true;
}
void Material::setMetallic(float value) {
    _metallic = value;
    _metallicImage = {};
    _updated = true;
}
void Material::setMetallicImage(StringId texture) {
    _metallicImage = texture;
    _updated = true;
}
void Material::setRoughness(float value) {
    _roughness = value;
    _roughnessImage = {};
    _updated = true;
}
void Material::setRoughnessImage(StringId texture) {
    _roughnessImage = texture;
    _updated = true;
}
void Material::setAo(float value) {
    _ao = value;
    _aoImage = {};
    _updated = true;
}
void Material::setAoImage(StringId texture) {
    _aoImage = texture;
    _updated = true;
}
void Material::setNormalImage(StringId texture) {
    _normalImage = texture;
    _updated = true;
}

bool Material::wasUpdated() {
    bool updated = _updated;
    _updated = false;
    return updated;
}

} // namespace atta::resource
