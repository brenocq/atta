//--------------------------------------------------
// Atta File Module
// projectSerializerSerialize.cpp
// Date: 2022-06-02
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::file {
void ProjectSerializer::serializeHeader(Section& section) {
    section["version"] = std::vector<uint16_t>{ATTA_VERSION_MAJOR, ATTA_VERSION_MINOR, ATTA_VERSION_PATCH, ATTA_VERSION_TWEAK};
    section["projectName"] = std::string(_project->getName());
}

void ProjectSerializer::serializeConfig(Section& section) { section["dt"] = float(Config::getDt()); }

void ProjectSerializer::serializeComponentSystem(Section& section) {
    // Serialize entity ids
    std::vector<EntityId> entities = ComponentManager::getNoCloneView();
    section["entityIds"] = entities;

    // Serialize components
    for (auto compReg : ComponentManager::getComponentRegistries()) {
        std::string name = compReg->getDescription().name;

        // Get all entities that have this component
        std::vector<EntityId> eids;
        std::vector<Component*> components;
        for (auto entity : entities) {
            Component* comp = ComponentManager::getEntityComponentById(compReg->getId(), entity);
            if (comp != nullptr) {
                eids.push_back(entity);
                components.push_back(comp);
            }
        }
        section["components"][name]["entityIds"] = eids;

        // Serialize components
        std::vector<uint8_t> componentsData;
        for (Component* component : components) {
            size_t size = compReg->getSerializedSize(component);
            // Create temporary buffer
            std::vector<uint8_t> temp(size);
            // Serialize to temporary buffer
            std::stringstream ss;
            compReg->serialize(ss, component);
            ss.seekg(0, std::ios_base::beg);
            ss.read((char*)temp.data(), temp.size());

            // Copy from temporary buffer to componentsData
            componentsData.insert(componentsData.end(), temp.begin(), temp.end());
        }
        section["components"][name]["data"] = componentsData;
    }
}

void ProjectSerializer::serializeGraphicsSystem(Section& section) {
    std::vector<std::shared_ptr<Viewport>> pviewports = GraphicsManager::getViewports();
    std::vector<Viewport> viewports;
    for (auto pv : pviewports)
        viewports.push_back(*pv);
    section["viewports"] = viewports;
}
} // namespace atta::file
