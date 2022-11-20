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
#include <atta/utils/config.h>

#include <atta/physics/interface.h>
#include <atta/physics/engines/bulletEngine.h>

namespace atta::file {

ProjectSerializer::ProjectSerializer(std::shared_ptr<Project> project) : _project(project) {}

ProjectSerializer::~ProjectSerializer() {}

void ProjectSerializer::serialize() {
    // Serialize to temporary file
    fs::path attaTemp = _project->getFile();
    attaTemp.replace_extension(".atta.temp");

    // Load data to section
    Section section;
    serializeHeader(section["header"]);
    serializeConfig(section["config"]);
    serializeComponentModule(section["componentModule"]);
    serializeGraphicsModule(section["graphicsModule"]);
    serializeResourceModule(section["resourceModule"]);
    serializePhysicsModule(section["physicsModule"]);
    // LOG_DEBUG("file::ProjectSerializer", "Saving project: [w]$0", section);

    // Serialize version
    std::ofstream os(attaTemp, std::ofstream::trunc | std::ofstream::binary);
    std::string version = "ATTA" + std::to_string(ATTA_VERSION_MAJOR) + "." + std::to_string(ATTA_VERSION_MINOR) + "." +
                          std::to_string(ATTA_VERSION_PATCH) + "." + std::to_string(ATTA_VERSION_TWEAK);
    write(os, version);

    // Serialize data
    section.serialize(os);
    os.close();

    // Override atta file with temp file
    fs::rename(attaTemp, _project->getFile());
}

void ProjectSerializer::deserialize() {
    Section section;
    fs::path attaFile = _project->getFile();

    std::ifstream is(attaFile, std::ifstream::in | std::ifstream::binary);

    // Deserialize version
    std::string version;
    read(is, version);
    section.deserialize(is);
    is.close();

    // Deserialize data
    deserializeHeader(section["header"]);
    deserializeConfig(section["config"]);
    deserializeComponentModule(section["componentModule"]);
    deserializeGraphicsModule(section["graphicsModule"]);
    deserializeResourceModule(section["resourceModule"]);
    deserializePhysicsModule(section["physicsModule"]);
}

} // namespace atta::file

#include <atta/file/project/projectSerializerDeserialize.cpp>
#include <atta/file/project/projectSerializerSerialize.cpp>
