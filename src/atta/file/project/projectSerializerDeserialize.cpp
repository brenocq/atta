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

void ProjectSerializer::deserializeComponentSystem(Section& section) {
    // Create entities
    std::vector<EntityId> entities = std::vector<EntityId>(section["entityIds"]);
    for (EntityId id : entities) {
        EntityId res = ComponentManager::createEntity(id);
        if (res != id)
            LOG_WARN("ProjectSerializer", "Could not create entity with id $0", id);
    }

    // Create and assign components
    for (auto compReg : ComponentManager::getComponentRegistries()) {
        std::string name = compReg->getDescription().name;

        // Get all entities that have this component
        if (section["components"].contains(name)) {
            std::vector<EntityId> eids = std::vector<EntityId>(section["components"][name]["entityIds"]);
            std::vector<uint8_t> componentsData = std::vector<uint8_t>(section["components"][name]["data"]);
            std::stringstream ss;
            write(ss, componentsData.data(), componentsData.size());

            for (EntityId eid : eids) {
                Component* component = ComponentManager::addEntityComponentById(compReg->getId(), eid);
                compReg->deserialize(ss, component);
            }
        }
    }
}

void ProjectSerializer::deserializeGraphicsSystem(Section& section) {
    std::vector<Viewport> viewports = std::vector<Viewport>(section["viewports"]);
    GraphicsManager::clearViewports();
    for (auto& viewport : viewports) {
        std::shared_ptr<Viewport> v = std::make_shared<Viewport>();
        *v = viewport;
        GraphicsManager::addViewport(v);
    }
}
} // namespace atta::file
