//--------------------------------------------------
// Atta File Module
// projectSerializer.cpp
// Date: 2021-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/cmakeConfig.h>
#include <atta/component/base.h>
#include <atta/component/components/components.h>
#include <atta/component/components/relationship.h>
#include <atta/component/interface.h>
#include <atta/file/project/projectSerializer.h>
#include <atta/file/serializer/section.h>
#include <atta/file/serializer/serializer.h>
#include <atta/graphics/interface.h>
#include <atta/physics/engines/bulletEngine.h>
#include <atta/physics/interface.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/image.h>
#include <atta/resource/resources/mesh.h>
#include <atta/sensor/interface.h>
#include <atta/ui/interface.h>
#include <atta/utils/config.h>
#include <atta/utils/stringUtils.h>

namespace atta::file {

ProjectSerializer::ProjectSerializer(std::shared_ptr<Project> project) : _project(project) {}

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

bool ProjectSerializer::deserialize() {
    fs::path attaFile = _project->getFile();
    Serializer serializer;
    serializer.deserialize(attaFile);

    // Make sure atta versions match
    bool canLoadProject = false;
    for (const Section& section : serializer.getSections()) {
        if (section.getName() == "project") {
            canLoadProject = deserializeProject(section);
            break;
        }
    }
    if (!canLoadProject)
        return false;

    for (const Section& section : serializer.getSections()) {
        if (section.getName() == "config")
            deserializeConfig(section);
        else if (section.getName() == "graphics")
            deserializeGraphicsModule(section);
        else if (section.getName() == "physics")
            deserializePhysicsModule(section);
        else if (section.getName() == "sensor")
            deserializeSensorModule(section);
        else if (section.getName() == "material")
            deserializeMaterial(section);
        else if (section.getName() == "node")
            deserializeNode(section);
    }
    return true;
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

#define ATTA_SERIALIZE_VECTOR(TYPE, CAST_TYPE)                                                                                                       \
    case cmp::AttributeType::VECTOR_##TYPE:                                                                                                          \
        section[dataKey] = std::vector<CAST_TYPE>(reinterpret_cast<CAST_TYPE*>(ptr), reinterpret_cast<CAST_TYPE*>(ptr) + size / sizeof(CAST_TYPE));  \
        break;

void serializeAttribute(Section& section, const std::string& cmpName, cmp::Component* data, const cmp::AttributeDescription& attribute, size_t size) {
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
            if (attribute.options.size() > 0)
                // Serialize uint32_t from enum as string
                section[dataKey] = attribute.options[*reinterpret_cast<uint32_t*>(ptr)];
            else
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
        case cmp::AttributeType::VECTOR_CHAR:
            section[dataKey] = std::string(reinterpret_cast<char*>(ptr));
            break;
            ATTA_SERIALIZE_VECTOR(BOOL, bool)
            ATTA_SERIALIZE_VECTOR(INT8, int8_t)
            ATTA_SERIALIZE_VECTOR(INT16, int16_t)
            ATTA_SERIALIZE_VECTOR(INT32, int32_t)
            ATTA_SERIALIZE_VECTOR(INT64, int64_t)
            ATTA_SERIALIZE_VECTOR(UINT8, uint8_t)
            ATTA_SERIALIZE_VECTOR(UINT16, uint16_t)
            ATTA_SERIALIZE_VECTOR(UINT32, uint32_t)
            ATTA_SERIALIZE_VECTOR(UINT64, uint64_t)
            ATTA_SERIALIZE_VECTOR(FLOAT32, float)
            ATTA_SERIALIZE_VECTOR(FLOAT64, double)
        case cmp::AttributeType::QUAT:
            section[dataKey] = *reinterpret_cast<quat*>(ptr);
            break;
        case cmp::AttributeType::STRINGID:
            section[dataKey] = reinterpret_cast<StringId*>(ptr)->getString();
            break;
        case cmp::AttributeType::CUSTOM:
            // Ignore custom attributes
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
    for (cmp::EntityId eid : entities) {
        Section section("node");
        section["id"] = eid;

        // Serialize components
        for (cmp::ComponentRegistry* compReg : cmp::getComponentRegistries()) {
            // Check if entity has this component
            cmp::Component* comp = cmp::getComponentById(compReg->getId(), eid);
            if (comp == nullptr)
                continue;
            // Serialize each attribute of the component
            std::string cmpName = toCamelCase(compReg->getDescription().name);

            if (cmpName == "relationship") {
                // TODO implement serialization of custom components
                cmp::Relationship* rel = cmp::Entity(eid).get<cmp::Relationship>();
                if (rel == nullptr)
                    continue;
                if (rel->getParent() != -1)
                    section[cmpName + ".parent"] = rel->getParent().getId();
            } else {
                const std::vector<cmp::AttributeDescription> attributeDescriptions = compReg->getDescription().attributeDescriptions;
                for (size_t i = 0; i < attributeDescriptions.size(); i++) {
                    cmp::AttributeDescription aDesc = attributeDescriptions[i];
                    size_t size = (i == attributeDescriptions.size() - 1) ? compReg->getSizeof() - aDesc.offset
                                                                          : attributeDescriptions[i + 1].offset - aDesc.offset;
                    serializeAttribute(section, cmpName, comp, aDesc, size);
                }
            }
        }

        sections.push_back(std::move(section));
    }
    return sections;
}

bool ProjectSerializer::deserializeProject(const Section& section) {
    if (!section.contains("attaVersion")) {
        LOG_WARN("file::ProjectSerializer", "Project [w]$0[] does not have an atta version. The project will not be loaded", _project->getName());
        return false;
    }
    std::string projectAttaVersion = std::string(section["attaVersion"]);
    if (projectAttaVersion != ATTA_VERSION) {
        LOG_WARN(
            "file::ProjectSerializer",
            "Project [w]$0[] was created with atta version [w]$1[], which does not match this atta version [w]$2[]. The project will not be loaded",
            _project->getName(), projectAttaVersion, ATTA_VERSION);
        return false;
    }

    // Load project name
    if (section.contains("name"))
        _project->_name = std::string(section["name"]);
    return true;
}

void ProjectSerializer::deserializeConfig(const Section& section) {
    if (section.contains("dt"))
        Config::setDt(float(section["dt"]));
    if (section.contains("desiredStepSpeed"))
        Config::setDesiredStepSpeed(float(section["desiredStepSpeed"]));
}

void ProjectSerializer::deserializeGraphicsModule(const Section& section) {
    if (section.contains("graphicsFPS"))
        gfx::setGraphicsFPS(float(section["graphicsFPS"]));
    if (section.contains("viewportRendering"))
        ui::setViewportRendering(bool(section["viewportRendering"]));
}

void ProjectSerializer::deserializePhysicsModule(const Section& section) {
    if (section.contains("engine"))
        physics::setEngineType(physics::Engine::stringToType.at(std::string(section["engine"])));
    if (section.contains("gravity"))
        physics::setGravity(vec3(section["gravity"]));
    if (section.contains("showColliders"))
        physics::setShowColliders(bool(section["showColliders"]));
    if (section.contains("showContacts"))
        physics::setShowContacts(bool(section["showContacts"]));
    if (section.contains("showJoints"))
        physics::setShowJoints(bool(section["showJoints"]));

    auto bullet = physics::getEngine<physics::BulletEngine>();
    if (section.contains("bullet.showAabb"))
        bullet->setShowAabb(bool(section["bullet.showAabb"]));
    if (section.contains("bullet.numSubSteps"))
        bullet->setNumSubSteps(unsigned(section["bullet.numSubSteps"]));
}

void ProjectSerializer::deserializeSensorModule(const Section& section) {
    if (section.contains("showCameras"))
        sensor::setShowCameras(bool(section["showCameras"]));
    if (section.contains("showInfrareds"))
        sensor::setShowInfrareds(bool(section["showInfrareds"]));
}

void ProjectSerializer::deserializeMaterial(const Section& section) {
    res::Material::CreateInfo material{};
    if (!section.contains("id")) {
        LOG_WARN("file::ProjectSerializer", "Material section does not have an id. The material will not be loaded");
        return;
    }
    std::string id = std::string(section["id"]);

    if (section.contains("color"))
        material.color = vec3f(section["color"]);
    if (section.contains("colorImage"))
        material.colorImage = StringId(std::string(section["colorImage"]));
    if (section.contains("metallic"))
        material.metallic = float(section["metallic"]);
    if (section.contains("metallicImage"))
        material.metallicImage = StringId(std::string(section["metallicImage"]));
    if (section.contains("roughness"))
        material.roughness = float(section["roughness"]);
    if (section.contains("roughnessImage"))
        material.roughnessImage = StringId(std::string(section["roughnessImage"]));
    if (section.contains("ao"))
        material.ao = float(section["ao"]);
    if (section.contains("aoImage"))
        material.aoImage = StringId(std::string(section["aoImage"]));
    if (section.contains("normalImage"))
        material.normalImage = StringId(std::string(section["normalImage"]));

    res::create<res::Material>(id, material);
}

#define ATTA_DESERIALIZE_VECTOR(TYPE, CAST_TYPE)                                                                                                     \
    case cmp::AttributeType::VECTOR_##TYPE: {                                                                                                        \
        std::vector<CAST_TYPE> vec = std::vector<CAST_TYPE>(section[dataKey]);                                                                       \
        std::copy(vec.begin(), vec.end(), reinterpret_cast<CAST_TYPE*>(ptr));                                                                        \
        break;                                                                                                                                       \
    }

void deserializeAttribute(const Section& section, const std::string& cmpName, cmp::Component* comp, const cmp::AttributeDescription& attribute) {
    std::string dataKey = cmpName + '.' + toCamelCase(attribute.name);
    if (!section.contains(dataKey))
        return;

    uint8_t* ptr = reinterpret_cast<uint8_t*>(comp) + attribute.offset; // Pointer to the attribute

    switch (attribute.type) {
        case cmp::AttributeType::BOOL:
            *reinterpret_cast<bool*>(ptr) = bool(section[dataKey]);
            break;
        case cmp::AttributeType::CHAR:
            *reinterpret_cast<char*>(ptr) = char(section[dataKey]);
            break;
        case cmp::AttributeType::INT8:
            *reinterpret_cast<int8_t*>(ptr) = int8_t(section[dataKey]);
            break;
        case cmp::AttributeType::INT16:
            *reinterpret_cast<int16_t*>(ptr) = int16_t(section[dataKey]);
            break;
        case cmp::AttributeType::INT32:
            *reinterpret_cast<int32_t*>(ptr) = int32_t(section[dataKey]);
            break;
        case cmp::AttributeType::INT64:
            *reinterpret_cast<int64_t*>(ptr) = int64_t(section[dataKey]);
            break;
        case cmp::AttributeType::UINT8:
            *reinterpret_cast<uint8_t*>(ptr) = uint8_t(section[dataKey]);
            break;
        case cmp::AttributeType::UINT16:
            *reinterpret_cast<uint16_t*>(ptr) = uint16_t(section[dataKey]);
            break;
        case cmp::AttributeType::UINT32:
            if (!attribute.options.empty()) {
                // Deserialize uint32_t from enum as string
                std::string value = std::string(section[dataKey]);
                auto it = std::find(attribute.options.begin(), attribute.options.end(), value);
                if (it != attribute.options.end())
                    *reinterpret_cast<uint32_t*>(ptr) = std::distance(attribute.options.begin(), it);
            } else {
                *reinterpret_cast<uint32_t*>(ptr) = uint32_t(section[dataKey]);
            }
            break;
        case cmp::AttributeType::UINT64:
            *reinterpret_cast<uint64_t*>(ptr) = uint64_t(section[dataKey]);
            break;
        case cmp::AttributeType::FLOAT32:
            *reinterpret_cast<float*>(ptr) = float(section[dataKey]);
            break;
        case cmp::AttributeType::FLOAT64:
            *reinterpret_cast<double*>(ptr) = double(section[dataKey]);
            break;
        case cmp::AttributeType::VECTOR_CHAR: {
            std::string value = std::string(section[dataKey]);
            std::copy(value.begin(), value.end(), reinterpret_cast<char*>(ptr));
            break;
        }
            ATTA_DESERIALIZE_VECTOR(BOOL, bool)
            ATTA_DESERIALIZE_VECTOR(INT8, int8_t)
            ATTA_DESERIALIZE_VECTOR(INT16, int16_t)
            ATTA_DESERIALIZE_VECTOR(INT32, int32_t)
            ATTA_DESERIALIZE_VECTOR(INT64, int64_t)
            ATTA_DESERIALIZE_VECTOR(UINT8, uint8_t)
            ATTA_DESERIALIZE_VECTOR(UINT16, uint16_t)
            ATTA_DESERIALIZE_VECTOR(UINT32, uint32_t)
            ATTA_DESERIALIZE_VECTOR(UINT64, uint64_t)
            ATTA_DESERIALIZE_VECTOR(FLOAT32, float)
            ATTA_DESERIALIZE_VECTOR(FLOAT64, double)
        case cmp::AttributeType::QUAT:
            *reinterpret_cast<quat*>(ptr) = quat(section[dataKey]);
            break;
        case cmp::AttributeType::STRINGID:
            *reinterpret_cast<StringId*>(ptr) = std::string(section[dataKey]);
            break;
        case cmp::AttributeType::CUSTOM:
            // Ignore custom attributes
            break;
        default:
            LOG_WARN("file::ProjectSerializer", "Attribute [w]$0.$1[] has unsupported type [w]$2[]. The attribute will not be deserialized", cmpName,
                     attribute.name, static_cast<int>(attribute.type));
            break;
    }
}

void ProjectSerializer::deserializeNode(const Section& section) {
    // Create entity
    cmp::Entity entity;
    if (section.contains("id")) {
        cmp::EntityId id = cmp::EntityId(section["id"]);

        // Check if entity doesn't already exist
        std::vector<cmp::EntityId> entities = cmp::getEntitiesView();
        bool alreadyExists = std::find(entities.begin(), entities.end(), id) != entities.end();
        if (!alreadyExists) {
            // Create entity if it doesn't exist
            cmp::EntityId res = cmp::createEntity(id);
            if (res != id) {
                LOG_WARN("file::ProjectSerializer", "Could not create entity with id $0", id);
                return;
            }
        }

        entity = id;
    } else
        return;

    // Iterate through all registered components
    for (cmp::ComponentRegistry* compReg : cmp::getComponentRegistries()) {
        std::string cmpName = toCamelCase(compReg->getDescription().name);

        if (cmpName == "relationship") {
            // TODO implement deserialization of custom components
            if (section.contains("relationship.parent")) {
                // Check if parent is valid
                cmp::EntityId parent = uint32_t(section["relationship.parent"]);
                if (parent == -1)
                    continue;

                // Create parent if it doesn't exist yet
                std::vector<cmp::EntityId> entities = cmp::getEntitiesView();
                bool parentExists = std::find(entities.begin(), entities.end(), parent) != entities.end();
                if (!parentExists)
                    cmp::createEntity(parent);

                // Update relationship component
                entity.add<cmp::Relationship>()->setParent(parent, entity);

                LOG_DEBUG("file::ProjectSerializer", "--------------> Entity [w]$0[] parent [w]$1", entity, parent);
            }
        } else {
            // Check if this component exists in the serialized section
            bool hasComponent = false;
            for (const cmp::AttributeDescription& attribute : compReg->getDescription().attributeDescriptions) {
                std::string dataKey = cmpName + '.' + toCamelCase(attribute.name);
                if (section.contains(dataKey)) {
                    hasComponent = true;
                    break;
                }
            }
            if (!hasComponent)
                continue;

            // Add component to entity
            cmp::Component* comp = cmp::addComponentById(compReg->getId(), entity);

            // Deserialize each attribute
            for (const cmp::AttributeDescription& attribute : compReg->getDescription().attributeDescriptions) {
                deserializeAttribute(section, cmpName, comp, attribute);
            }
        }
    }
}

} // namespace atta::file
