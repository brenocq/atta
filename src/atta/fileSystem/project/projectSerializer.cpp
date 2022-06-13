//--------------------------------------------------
// Atta File System
// projectSerializer.cpp
// Date: 2021-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/fileSystem/project/projectSerializer.h>
#include <atta/fileSystem/serializer/serializer.h>
#include <atta/fileSystem/serializer/section.h>
#include <atta/cmakeConfig.h>

#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/components.h>

#include <atta/resourceSystem/resourceManager.h>
#include <atta/resourceSystem/resources/mesh.h>
#include <atta/resourceSystem/resources/texture.h>

#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/core/config.h>

namespace atta
{
    ProjectSerializer::ProjectSerializer(std::shared_ptr<Project> project):
        _project(project)
    {
    }

    ProjectSerializer::~ProjectSerializer()
    {
    }

    void ProjectSerializer::serialize()
    {
        // Serialize to temporary file
        fs::path attaTemp = _project->getFile();
        attaTemp.replace_extension(".atta.temp");

        // Load data to section 
        Section section;
        serializeHeader(section["header"]);
        serializeConfig(section["config"]);
        serializeComponentSystem(section["componentSystem"]);
        serializeGraphicsSystem(section["graphicsSystem"]);
        //LOG_DEBUG("ProjectSerializer", "Saving project: [w]$0", section);

        // Serialize version
        std::ofstream os(attaTemp, std::ofstream::trunc | std::ofstream::binary);
        std::string version = "ATTA" + 
            std::to_string(ATTA_VERSION_MAJOR) + "." + 
            std::to_string(ATTA_VERSION_MINOR) + "." +
            std::to_string(ATTA_VERSION_PATCH) + "." +
            std::to_string(ATTA_VERSION_TWEAK);
        write(os, version);

        // Serialize data
        section.serialize(os);
        os.close();

        // Override atta file with temp file
        fs::rename(attaTemp, _project->getFile());
    }

    void ProjectSerializer::deserialize()
    {
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
        deserializeComponentSystem(section["componentSystem"]);
        deserializeGraphicsSystem(section["graphicsSystem"]);
    }
}

#include <atta/fileSystem/project/projectSerializerSerialize.cpp>
#include <atta/fileSystem/project/projectSerializerDeserialize.cpp>
