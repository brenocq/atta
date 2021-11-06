//--------------------------------------------------
// Atta File System
// projectSerializer.cpp
// Date: 2021-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/fileSystem/project/projectSerializer.h>
#include <atta/cmakeConfig.h>

#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/components.h>

#include <atta/resourceSystem/resourceManager.h>
#include <atta/resourceSystem/resources/mesh.h>
#include <atta/resourceSystem/resources/texture.h>

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
        fs::path attaTemp = _project->getFile();
        attaTemp.replace_extension(".atta.temp");

        std::ofstream os(attaTemp, std::ofstream::trunc | std::ofstream::binary);

        serializeHeader(os);
        serializeComponentSystem(os);

        os.close();

        // Override atta file with temp file
        fs::rename(attaTemp, _project->getFile());
    }

    void ProjectSerializer::deserialize()
    {
        fs::path attaFile = _project->getFile();

        std::ifstream is(attaFile, std::ifstream::in | std::ifstream::binary);

        Header header = deserializeHeader(is);
        //LOG_WARN("ProjectSerializer", "[*y]Header:[]\n\tversion:$0.$1.$2.$3\n\tproj:$4\n\tcounter:$5", 
        //		header.version[0], header.version[1], header.version[2], header.version[3], 
        //		header.projectName, header.saveCounter);

        if(is)
        {
            std::string marker;
            read(is, marker);
            if(marker == "comp")
                deserializeComponentSystem(is);
        }

        is.close();
    }

    void ProjectSerializer::serializeHeader(std::ofstream& os)
    {
        // Atta info
        write(os, "atta");
        uint16_t version[] = { ATTA_VERSION_MAJOR, ATTA_VERSION_MINOR, ATTA_VERSION_PATCH, ATTA_VERSION_TWEAK };
        write(os, version);// Version

        // Project info
        write(os, "proj");
        write(os, _project->getName());// Project name
        uint32_t saveCounter = 0;
        write(os, saveCounter);// Save counter (number of times that was saved)
        write(os, "hend");
    }

    ProjectSerializer::Header ProjectSerializer::deserializeHeader(std::ifstream& is)
    {
        std::string marker;
        Header header;

        while(true)
        {
            read(is, marker);
            if(marker == "hend")
            {
                return header;
            }
            else if(marker == "atta")
            {
                read(is, header.version);
            }
            else if(marker == "proj")
            {
                read(is, header.projectName);
                read(is, header.saveCounter);
            }
            else
            {
                LOG_WARN("ProjectSerializer", "Unknown marker found at the header [w]$0[]", marker);
                return header;
            }
        }
        return header;
    }

    void ProjectSerializer::serializeComponentSystem(std::ofstream& os)
    {
        // Component system marker
        write(os, "comp");

        // Serialize entity ids
        std::vector<EntityId> entities = ComponentManager::getEntities();
        write(os, "id");// Entity id marker
        write<uint32_t>(os, entities.size());
        for(EntityId entity : entities)
            write(os, entity);

        // Serialize number of components
        write<uint32_t>(os, ComponentManager::getComponentRegistries().size());

        // Serialize components
        for(auto compReg : ComponentManager::getComponentRegistries())
        {
            // Write name
            write(os, compReg->getDescription().type);

            // Get all entities that have this component
            std::vector<std::pair<EntityId,Component*>> pairs;
            for(auto entity : entities)
            {
                Component* comp = ComponentManager::getEntityComponentById(compReg->getId(), entity);
                if(comp != nullptr)
                    pairs.push_back(std::make_pair(entity, comp));
            }

            // Write size in bytes of this next section (can be useful if the component is unknown and can't be deserialized)
            unsigned totalSectionSize = 0;
            for(auto compPair : pairs)
                totalSectionSize += compReg->getSerializedSize(compPair.second) + sizeof(EntityId);
            write<uint32_t>(os, totalSectionSize);

            for(auto compPair : pairs)
            {
                // TODO StringId as string
                // TODO Open vector
                write(os, compPair.first);
                compReg->serialize(os, compPair.second);
            }
        }
    }

    void ProjectSerializer::deserializeComponentSystem(std::ifstream& is)
    {
        std::string marker;
        uint32_t numComponents = 0;

        // Read entity ids
        {
            read(is, marker);
            if(marker != "id")
            {
                LOG_WARN("ProjectSerializer", "First component marker must be [w]id[], but it is [w]$0[]. Could not load atta file", marker);
                return;
            }

            uint32_t numIds;
            read(is, numIds);
            LOG_VERBOSE("ProjectSerializer","Num ids: $0", numIds);
            for(uint32_t i = 0; i < numIds; i++)
            {
                EntityId id;
                read(is, id);
                LOG_VERBOSE("ProjectSerializer","id: $0", id);

                EntityId res = ComponentManager::createEntity();
                if(res != id)
                    LOG_VERBOSE("ProjectSerializer","Created and serialized entities does not match! $0 and $1", id, res);
            }
        }

        // Read number of components
        read(is, numComponents);
        LOG_VERBOSE("ProjectSerializer","Num components: $0", numComponents);

        for(uint32_t i = 0; i < numComponents; i++)
        {
            uint32_t sectionSize;// Can be used to skip the component data if it is an unknown component

            // Read component name
            read(is, marker);
            read(is, sectionSize);
            ComponentRegistry* compReg = nullptr;
            LOG_VERBOSE("ProjectSerializer"," - Component $0 size: $1", marker, sectionSize);

            // Find correct component registry
            for(auto componentRegistry : ComponentManager::getComponentRegistries())
            {
                if(componentRegistry->getDescription().type == marker)
                {
                    compReg = componentRegistry;
                    break;
                }
            }

            // If not found the component, skip this section
            if(compReg == nullptr)
            {
                LOG_WARN("ProjectSerializer"," - Ignoring component $0, it is unknown");
                is.ignore(sectionSize);
                continue;
            }

            // Deserialize 
            size_t endPos = size_t(is.tellg()) + sectionSize;
            LOG_DEBUG("ProjectSerializer", "curr: $0 end: $1", is.tellg(), endPos);
            for(; size_t(is.tellg()) < endPos; i++)
            {
                EntityId eid;
                read(is, eid);
                LOG_WARN("ProjectSerializer", " - Data eid: $0", eid);
                Component* component = ComponentManager::addEntityComponentById(compReg->getId(), eid);
                LOG_WARN("ProjectSerializer", " - Component added");
                compReg->deserialize(is, component);
                LOG_WARN("ProjectSerializer", " - Read!");
            }
        }
    }
}

