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

void ProjectSerializer::serializeComponentModule(Section& section) {
    // Serialize entity ids
    std::vector<component::EntityId> entities = component::getNoCloneView();
    section["entityIds"] = entities;

    // Serialize components
    for (auto compReg : component::getComponentRegistries()) {
        std::string name = compReg->getDescription().name;

        // Get all entities that have this component
        std::vector<component::EntityId> eids;
        std::vector<component::Component*> components;
        for (auto entity : entities) {
            component::Component* comp = component::getComponentById(compReg->getId(), entity);
            if (comp != nullptr) {
                eids.push_back(entity);
                components.push_back(comp);
            }
        }
        if (eids.size()) {
            section["components"][name]["entityIds"] = eids;

            // Serialize components
            std::vector<uint8_t> componentsData;
            for (component::Component* component : components) {
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
}

void ProjectSerializer::serializeGraphicsModule(Section& section) {
    std::vector<std::shared_ptr<graphics::Viewport>> pviewports = graphics::getViewports();
    std::vector<graphics::Viewport> viewports;
    for (auto pv : pviewports)
        viewports.push_back(*pv);
    section["viewports"] = viewports;
    section["graphicsFPS"] = graphics::getGraphicsFPS();
    section["viewportFPS"] = graphics::getViewportFPS();
    section["viewportRendering"] = graphics::getViewportRendering();
}

void ProjectSerializer::serializeResourceModule(Section& section) {
    std::vector<StringId> materialSids = resource::getResources<resource::Material>();
    std::vector<resource::Material> materials;
    for (StringId sid : materialSids) {
        resource::Material* m = resource::get<resource::Material>(sid.getString());
        if (m)
            materials.push_back(*m);
    }
    section["materials"] = materials;
}

void ProjectSerializer::serializePhysicsModule(Section& section) {
    switch (physics::getSelectedEngine()) {
        case physics::Engine::NONE:
            section["engine"] = "NONE";
            break;
        case physics::Engine::BOX2D:
            section["engine"] = "BOX2D";
            break;
        case physics::Engine::BULLET:
            section["engine"] = "BULLET";
            break;
    }
}

} // namespace atta::file
