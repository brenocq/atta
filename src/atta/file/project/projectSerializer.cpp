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

#include <atta/component/base.h>
#include <atta/component/components/components.h>
#include <atta/component/interface.h>

#include <atta/resource/interface.h>
#include <atta/resource/resources/image.h>
#include <atta/resource/resources/mesh.h>

#include <atta/graphics/interface.h>
#include <atta/ui/interface.h>
#include <atta/utils/config.h>
#include <atta/utils/stringUtils.h>

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
    section["attaVersion"] = std::string{ATTA_VERSION};
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
    Section section("sensor");
    section["showCameras"] = sensor::getShowCameras();
    section["showInfrareds"] = sensor::getShowInfrareds();
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

void serializeAttribute(Section& section, const std::string& cmpName, cmp::Component* data, const cmp::AttributeDescription& attribute) {
    std::string dataKey = cmpName + '.' + toCamelCase(attribute.name);
    uint8_t* ptr = reinterpret_cast<uint8_t*>(data) + attribute.offset; // Base pointer to the attribute

    switch (attribute.type) {
        case cmp::AttributeType::BOOL:
            section[dataKey] = *reinterpret_cast<bool*>(ptr);
            break;
        case cmp::AttributeType::CHAR:
            section[dataKey] = *reinterpret_cast<char*>(ptr);
            break;
        case cmp::AttributeType::INT8:
            section[dataKey] = *reinterpret_cast<int8_t*>(ptr);
            break;
        case cmp::AttributeType::INT16:
            section[dataKey] = *reinterpret_cast<int16_t*>(ptr);
            break;
        case cmp::AttributeType::INT32:
            section[dataKey] = *reinterpret_cast<int32_t*>(ptr);
            break;
        case cmp::AttributeType::INT64:
            section[dataKey] = *reinterpret_cast<int64_t*>(ptr);
            break;
        case cmp::AttributeType::UINT8:
            section[dataKey] = *reinterpret_cast<uint8_t*>(ptr);
            break;
        case cmp::AttributeType::UINT16:
            section[dataKey] = *reinterpret_cast<uint16_t*>(ptr);
            break;
        case cmp::AttributeType::UINT32:
            section[dataKey] = *reinterpret_cast<uint32_t*>(ptr);
            break;
        case cmp::AttributeType::UINT64:
            section[dataKey] = *reinterpret_cast<uint64_t*>(ptr);
            break;
        case cmp::AttributeType::FLOAT32:
            section[dataKey] = *reinterpret_cast<float*>(ptr);
            break;
        case cmp::AttributeType::FLOAT64:
            section[dataKey] = *reinterpret_cast<double*>(ptr);
            break;
        case cmp::AttributeType::QUAT:
            section[dataKey] = *reinterpret_cast<quat*>(ptr);
            break;
        default:
            LOG_WARN("file::ProjectSerializer", "Attribute [w]$0.$1[] has unsupported type [w]$2[]. The attribute will not be saved to .atta",
                     cmpName, attribute.name, static_cast<int>(attribute.type));
            break;
    }
}

std::vector<Section> ProjectSerializer::serializeNodes() {
    std::vector<Section> sections;
    std::vector<cmp::EntityId> entities = cmp::getNoCloneView();
    // Serialize each entity as a node
    for (cmp::EntityId entity : entities) {
        Section section("node");
        section["id"] = entity;

        // Serialize components
        for (cmp::ComponentRegistry* compReg : cmp::getComponentRegistries()) {
            // Check if entity has this component
            cmp::Component* comp = cmp::getComponentById(compReg->getId(), entity);
            if (comp == nullptr)
                continue;
            // Serialize each attribute of the component
            std::string cmpName = toCamelCase(compReg->getDescription().name);
            for (const cmp::AttributeDescription& attribute : compReg->getDescription().attributeDescriptions)
                serializeAttribute(section, cmpName, comp, attribute);
        }

        sections.push_back(std::move(section));
    }
    return sections;
}

void ProjectSerializer::deserializeProject(Section& section) {}
void ProjectSerializer::deserializeConfig(Section& section) {}
void ProjectSerializer::deserializeGraphicsModule(Section& section) {}
void ProjectSerializer::deserializeResourceModule(Section& section) {}
void ProjectSerializer::deserializePhysicsModule(Section& section) {}
void ProjectSerializer::deserializeSensorModule(Section& section) {}
void ProjectSerializer::deserializeNode(Section& section) {}

} // namespace atta::file
