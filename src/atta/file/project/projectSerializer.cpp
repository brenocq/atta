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
    serializer.addSection(serializeResourceModule());
    serializer.addSection(serializePhysicsModule());
    serializer.addSection(serializeSensorModule());
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
    return section;
}

Section ProjectSerializer::serializeResourceModule() {
    Section section("resource");
    return section;
}

Section ProjectSerializer::serializePhysicsModule() {
    Section section("physics");
    return section;
}

Section ProjectSerializer::serializeSensorModule() {
    Section section("physics");
    return section;
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
