//--------------------------------------------------
// Atta File Module
// projectSerializerDeserialize.cpp
// Date: 2022-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::file {

void ProjectSerializer::deserializeHeader(Section& section) {
    std::vector<uint16_t> version = std::vector<uint16_t>(section["version"]);
    std::string projectName = std::string(section["projectName"]);
}

void ProjectSerializer::deserializeConfig(Section& section) { Config::setDt(float(section["dt"])); }

void ProjectSerializer::deserializeComponentModule(Section& section) {
    // Create entities
    std::vector<component::EntityId> entities = std::vector<component::EntityId>(section["entityIds"]);
    for (auto id : entities) {
        component::EntityId res = component::createEntity(id);
        if (res != id)
            LOG_WARN("file::ProjectSerializer", "Could not create entity with id $0", id);
    }

    // Create and assign components
    for (auto compReg : component::getComponentRegistries()) {
        std::string name = compReg->getDescription().name;

        // Get all entities that have this component
        if (section["components"].contains(name)) {
            std::vector<component::EntityId> eids = std::vector<component::EntityId>(section["components"][name]["entityIds"]);
            std::vector<uint8_t> componentsData = std::vector<uint8_t>(section["components"][name]["data"]);
            std::stringstream ss;
            write(ss, componentsData.data(), componentsData.size());

            for (auto eid : eids) {
                component::Component* component = component::addEntityComponentById(compReg->getId(), eid);
                compReg->deserialize(ss, component);
            }
        }
    }
}

void ProjectSerializer::deserializeGraphicsModule(Section& section) {
    std::vector<graphics::Viewport> viewports = std::vector<graphics::Viewport>(section["viewports"]);
    graphics::clearViewports();
    for (auto& viewport : viewports) {
        std::shared_ptr<graphics::Viewport> v = std::make_shared<graphics::Viewport>();
        *v = viewport;
        graphics::addViewport(v);
    }
}

void ProjectSerializer::deserializeResourceModule(Section& section) {
    std::vector<resource::Material> materials = std::vector<resource::Material>(section["materials"]);
    resource::destroyResources<resource::Material>();
    for (resource::Material material : materials) {
        resource::Material::CreateInfo info {};
        info.color = material.color;
        info.metallic = material.metallic;
        info.roughness = material.roughness;
        info.ao = material.ao;
        info.colorImage = material.colorImage;
        info.metallicImage = material.metallicImage;
        info.roughnessImage = material.roughnessImage;
        info.aoImage = material.aoImage;
        resource::create<resource::Material>(material.getId().getString(), info);
    }
}

} // namespace atta::file
