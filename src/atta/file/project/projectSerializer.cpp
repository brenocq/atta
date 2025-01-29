//--------------------------------------------------
// Atta File Module
// projectSerializer.cpp
// Date: 2021-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/cmakeConfig.h>
#include <atta/file/project/projectSerializer.h>
#include <atta/file/serializer/section.h>
#include <atta/file/serializer/serializer.h>

#include <atta/component/components/components.h>
#include <atta/component/interface.h>

#include <atta/resource/interface.h>
#include <atta/resource/resources/image.h>
#include <atta/resource/resources/mesh.h>

#include <atta/graphics/interface.h>
#include <atta/ui/interface.h>
#include <atta/utils/config.h>

#include <atta/physics/engines/bulletEngine.h>
#include <atta/physics/interface.h>

#include <atta/sensor/interface.h>

namespace atta::file {

ProjectSerializer::ProjectSerializer(std::shared_ptr<Project> project) : _project(project) {}

ProjectSerializer::~ProjectSerializer() {}

void ProjectSerializer::serialize() {
    Serializer serializer;
    serializer.addSection(serializeProject());
    serializer.addSection(serializeConfig());
    serializer.addSection(serializeGraphicsModule());
    serializer.addSection(serializePhysicsModule());
    serializer.addSection(serializeSensorModule());

    std::vector<Section> resources = serializeResources();
    for (const Section& resource : resources)
        serializer.addSection(resource);

    std::vector<Section> nodes = serializeNodes();
    for (const Section& node : nodes)
        serializer.addSection(node);

    // Serialize to temporary file
    fs::path attaTemp = _project->getFile();
    attaTemp.replace_extension(".atta.temp");
    serializer.serialize(attaTemp);

    // Override atta file with temp file
    fs::rename(attaTemp, _project->getFile());
    LOG_SUCCESS("file::ProjectSerializer", "Project [w]$0[] was saved", _project->getName());
}

void ProjectSerializer::deserialize() {
    Section section("Section");
    fs::path attaFile = _project->getFile();

    std::ifstream is(attaFile, std::ifstream::in | std::ifstream::binary);

    // Deserialize version
    // std::string version;
    // read(is, version);
    // section.deserialize(is);
    // is.close();

    // Deserialize data
    // deserializeHeader(section["header"]);
    // deserializeConfig(section["config"]);
    // deserializeComponentModule(section["componentModule"]);
    // deserializeGraphicsModule(section["graphicsModule"]);
    // deserializeResourceModule(section["resourceModule"]);
    // deserializePhysicsModule(section["physicsModule"]);
    // deserializeSensorModule(section["sensorModule"]);
}

Section ProjectSerializer::serializeProject() {
    Section section("project");
    section["attaVersion"] = std::vector<int>{ATTA_VERSION_MAJOR, ATTA_VERSION_MINOR, ATTA_VERSION_PATCH};
    section["name"] = _project->getName();
    return section;
}

Section ProjectSerializer::serializeConfig() {
    Section section("config");
    section["dt"] = Config::getDt();
    section["desiredStepSpeed"] = Config::getDesiredStepSpeed();
    return section;
}

Section ProjectSerializer::serializeGraphicsModule() {
    Section section("graphics");
    // std::vector<std::shared_ptr<ui::Viewport>> pviewports = ui::getViewports();
    // std::vector<ui::Viewport> viewports;
    // for (auto& pv : pviewports)
    //     viewports.push_back(*pv);
    // section["viewports"] = viewports;
    // LOG_WARN("file::ProjectSerializer", "Serializing viewports was not implemented yet");
    section["graphicsFPS"] = gfx::getGraphicsFPS();
    section["viewportRendering"] = ui::getViewportRendering();
    return section;
}

Section ProjectSerializer::serializePhysicsModule() {
    Section section("physics");

    section["engine"] = physics::Engine::typeToString.at(physics::getEngineType());
    section["gravity"] = physics::getGravity();
    section["showColliders"] = physics::getShowColliders();
    section["showContacts"] = physics::getShowContacts();
    section["showJoints"] = physics::getShowJoints();

    auto bullet = physics::getEngine<physics::BulletEngine>();
    section["bullet.showAabb"] = bullet->getShowAabb();
    section["bullet.numSubSteps"] = bullet->getNumSubSteps();

    return section;
}

Section ProjectSerializer::serializeSensorModule() {
    Section section("physics");
    return section;
}

std::vector<Section> ProjectSerializer::serializeResources() {
    std::vector<Section> sections;

    // Serialize materials
    res::Material::CreateInfo defaultMaterial{};
    std::vector<StringId> materialSids = res::getResources<res::Material>();
    for (StringId sid : materialSids) {
        res::Material* m = res::get<res::Material>(sid.getString());
        Section section("material");

        // Id
        section["id"] = sid.getString();

        // Serialize color if it is not the default
        if (m->getColor() != defaultMaterial.color)
            section["color"] = m->getColor();

        // Serialize color texture if used
        if (m->colorIsImage())
            section["colorImage"] = m->getColorImage().getString();

        // Serialize metallic factor or image
        if (m->getMetallic() != defaultMaterial.metallic)
            section["metallic"] = m->getMetallic();
        if (m->metallicIsImage())
            section["metallicImage"] = m->getMetallicImage().getString();

        // Serialize roughness factor or image
        if (m->getRoughness() != defaultMaterial.roughness)
            section["roughness"] = m->getRoughness();
        if (m->roughnessIsImage())
            section["roughnessImage"] = m->getRoughnessImage().getString();

        // Serialize ambient occlusion factor or image
        if (m->getAo() != defaultMaterial.ao)
            section["ao"] = m->getAo();
        if (m->aoIsImage())
            section["aoImage"] = m->getAoImage().getString();

        // Serialize normal map if it exists
        if (m->hasNormalImage())
            section["normalImage"] = m->getNormalImage().getString();

        sections.push_back(std::move(section));
    }

    return sections;
}

std::vector<Section> ProjectSerializer::serializeNodes() { return {}; }

void ProjectSerializer::deserializeProject(Section& section) {}
void ProjectSerializer::deserializeConfig(Section& section) {}
void ProjectSerializer::deserializeGraphicsModule(Section& section) {}
void ProjectSerializer::deserializeResourceModule(Section& section) {}
void ProjectSerializer::deserializePhysicsModule(Section& section) {}
void ProjectSerializer::deserializeSensorModule(Section& section) {}
void ProjectSerializer::deserializeNode(Section& section) {}

} // namespace atta::file
