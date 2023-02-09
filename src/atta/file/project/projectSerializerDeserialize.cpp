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

void ProjectSerializer::deserializeConfig(Section& section) {
    if (section.contains("dt"))
        Config::setDt(float(section["dt"]));
    if (section.contains("desiredStepSpeed"))
        Config::setDesiredStepSpeed(float(section["desiredStepSpeed"]));
}

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
                component::Component* component = component::addComponentById(compReg->getId(), eid);
                compReg->deserialize(ss, component);
            }
        }
    }
}

void ProjectSerializer::deserializeGraphicsModule(Section& section) {
    if (section.contains("viewports")) {
        std::vector<graphics::Viewport> viewports = std::vector<graphics::Viewport>(section["viewports"]);
        graphics::clearViewports();
        for (auto& viewport : viewports) {
            std::shared_ptr<graphics::Viewport> v = std::make_shared<graphics::Viewport>();
            *v = viewport;
            graphics::addViewport(v);
        }
    }
    if (section.contains("graphicsFPS"))
        graphics::setGraphicsFPS(float(section["graphicsFPS"]));
    if (section.contains("viewportFPS"))
        graphics::setViewportFPS(float(section["viewportFPS"]));
    if (section.contains("viewportRendering"))
        graphics::setViewportRendering(bool(section["viewportRendering"]));
}

void ProjectSerializer::deserializeResourceModule(Section& section) {
    std::vector<resource::Material> materials = std::vector<resource::Material>(section["materials"]);
    resource::destroyResources<resource::Material>();
    for (resource::Material material : materials) {
        resource::Material::CreateInfo info{};
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

void ProjectSerializer::deserializePhysicsModule(Section& section) {
    if (section.contains("engine")) {
        std::string engine = std::string(section["engine"]);
        if (engine == "NONE")
            physics::setEngineType(physics::Engine::NONE);
        else if (engine == "BOX2D")
            physics::setEngineType(physics::Engine::BOX2D);
        else if (engine == "BULLET")
            physics::setEngineType(physics::Engine::BULLET);
    }
    if (section.contains("gravity"))
        physics::setGravity(vec3(section["gravity"]));
    if (section.contains("showColliders"))
        physics::setShowColliders(bool(section["showColliders"]));
    if (section.contains("showContacts"))
        physics::setShowContacts(bool(section["showContacts"]));
    if (section.contains("showJoints"))
        physics::setShowJoints(bool(section["showJoints"]));

    if (section.contains("bullet")) {
        auto bullet = physics::getEngine<physics::BulletEngine>();
        if (section["bullet"].contains("showAabb"))
            bullet->setShowAabb(bool(section["bullet"]["showAabb"]));
        if (section["bullet"].contains("numSubSteps"))
            bullet->setNumSubSteps(unsigned(section["bullet"]["numSubSteps"]));
    }
}

void ProjectSerializer::deserializeSensorModule(Section& section) {
    if (section.contains("showCameras"))
        sensor::setShowCameras(bool(section["showCameras"]));
    if (section.contains("showInfrareds"))
        sensor::setShowInfrareds(bool(section["showInfrareds"]));
}

void ProjectSerializer::deserializeParallelModule(Section& section) {
    if (section.contains("device")) {
        std::string device = std::string(section["device"]);
        if (device == "SERIAL")
            parallel::setDeviceType(parallel::Device::SERIAL);
        else if (device == "CPU")
            parallel::setDeviceType(parallel::Device::CPU);
        else if (device == "GPU")
            parallel::setDeviceType(parallel::Device::GPU);
        else if (device == "CLUSTER")
            parallel::setDeviceType(parallel::Device::CLUSTER);
    }
}

} // namespace atta::file
