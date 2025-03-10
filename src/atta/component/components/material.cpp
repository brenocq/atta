//--------------------------------------------------
// Atta Component Module
// material.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/componentRegistry.h>
#include <atta/component/components/material.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/image.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<Material>::getDescription() {
    static ComponentDescription desc = {
        "Material", {{AttributeType::STRINGID, offsetof(Material, sid), "sid", {}, {}, {}, {}}},
        1024, // Max instances
        {},   // Serialize
        {},   // Deserialize
    };

    return desc;
}

resource::Material* Material::getResource() const { return resource::get<resource::Material>(sid.getString()); }
void Material::setResource(const resource::Material* material) { set(material); }

void Material::set(StringId material) { sid = material; }

void Material::set(const resource::Material* material) {
    if (material == nullptr) {
        LOG_WARN("component::Material", "Could not set resource because it is nullptr");
        return;
    }
    sid = material->getId();
}

} // namespace atta::component
