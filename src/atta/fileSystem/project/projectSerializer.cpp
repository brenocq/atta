//--------------------------------------------------
// Atta File System
// projectSerializer.cpp
// Date: 2021-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/fileSystem/project/projectSerializer.h>
#include <atta/fileSystem/serializer/serializer.h>
#include <atta/cmakeConfig.h>

#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/components.h>

#include <atta/resourceSystem/resourceManager.h>
#include <atta/resourceSystem/resources/mesh.h>
#include <atta/resourceSystem/resources/texture.h>

#include <atta/graphicsSystem/graphicsManager.h>

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
        serializeGraphicsSystem(os);
        write(os, "end");

        os.close();

        // Override atta file with temp file
        fs::rename(attaTemp, _project->getFile());
    }

    void ProjectSerializer::deserialize()
    {
        fs::path attaFile = _project->getFile();

        std::ifstream is(attaFile, std::ifstream::in | std::ifstream::binary);

        Header header = deserializeHeader(is);
        LOG_VERBOSE("ProjectSerializer", "[*y]Header:[]\n\tversion:$0.$1.$2.$3\n\tproj:$4\n\tcounter:$5", 
        		header.version[0], header.version[1], header.version[2], header.version[3], 
        		header.projectName, header.saveCounter);

        while(is)
        {
            std::string marker;
            read(is, marker);
            if(marker == "comp")
                deserializeComponentSystem(is);
            else if(marker == "gfx")
                deserializeGraphicsSystem(is);
            else if(marker == "end")
                break;
            else
            {
                LOG_WARN("ProjectSerializer", "Unknown marker [w]$0[]. The file may be corrupted", marker);
                break;
            }
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
        std::stringstream oss;
        std::basic_ostream<char>::pos_type posBefore = oss.tellp();

        //---------- Write to temporary buffer ----------//
        // Serialize entity ids
        std::vector<EntityId> entities = ComponentManager::getEntitiesView();
        write(oss, "id");// Entity id marker
        write<uint64_t>(oss, entities.size());
        for(EntityId entity : entities)
            write(oss, entity);

        // Serialize number of components
        write<uint32_t>(oss, ComponentManager::getComponentRegistries().size());

        // Serialize components
        for(auto compReg : ComponentManager::getComponentRegistries())
        {
            // Write name
            write(oss, compReg->getDescription().type);

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
            write<uint32_t>(oss, totalSectionSize);

            std::basic_ostream<char>::pos_type posBef = oss.tellp();
            for(auto compPair : pairs)
            {
                // TODO Open vector
                write(oss, compPair.first);
                compReg->serialize(oss, compPair.second);
            }
            ASSERT(oss.tellp()-posBef == totalSectionSize, "Serialized section size and calculated section size does not match");
        }

        //---------- Flush buffer to file ----------//
        // Write section header
        size_t size = (int)oss.tellp() - posBefore;
        write(os, "comp");// Section title
        write(os, size);// Section size

        // Write section body
        char* buffer = new char[size];
        oss.rdbuf()->pubseekpos(0);// Return to first position
        oss.rdbuf()->sgetn(buffer, size);// Copy to buffer
        os.write(reinterpret_cast<const char*>(buffer), size);
        delete buffer;
    }

    void ProjectSerializer::deserializeComponentSystem(std::ifstream& is)
    {
        // Read section size in bytes
        size_t sizeBytes;
        read(is, sizeBytes);

        // Read section
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

            uint64_t numIds;
            read(is, numIds);
            for(uint64_t i = 0; i < numIds; i++)
            {
                EntityId id;
                read(is, id);

                EntityId res = ComponentManager::createEntity();
                if(res != id)
                    LOG_WARN("ProjectSerializer","Created and serialized entities does not match! $0 and $1", id, res);
            }
        }

        // Read number of components
        read(is, numComponents);

        for(uint32_t i = 0; i < numComponents; i++)
        {
            uint32_t sectionSize;// Can be used to skip the component data if it is an unknown component

            // Read component name
            read(is, marker);
            read(is, sectionSize);
            ComponentRegistry* compReg = nullptr;

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
                is.ignore(sectionSize);
                continue;
            }

            // Deserialize 
            size_t endPos = size_t(is.tellg()) + sectionSize;
            while(size_t(is.tellg()) < endPos)
            {
                EntityId eid;
                read(is, eid);
                Component* component = ComponentManager::addEntityComponentById(compReg->getId(), eid);
                compReg->deserialize(is, component);
            }
        }
    }

    void ProjectSerializer::serializeGraphicsSystem(std::ofstream& os)
    {
        std::stringstream oss;
        std::basic_ostream<char>::pos_type posBefore = oss.tellp();

        //---------- Write to temporary buffer ----------//
        // Number of subsections
        write<uint32_t>(oss, 1);
        //----- Viewport subsection -----//
        write(oss, "viewports");

        std::vector<std::shared_ptr<Viewport>> viewports = GraphicsManager::getViewports();
        unsigned sectionSize = 0;
        for(auto viewport : viewports)
            sectionSize += viewport->getSerializedSize();
        // Serialize section size in bytes
        write<uint32_t>(oss, sectionSize);

        // Serialize number of viewports
        write<uint32_t>(oss, viewports.size());

        // Serialize 
        for(auto viewport : viewports)
            viewport->serialize(oss);

        //---------- Flush buffer to file ----------//
        // Write section header
        size_t size = (int)oss.tellp() - posBefore;
        write(os, "gfx");// Section title
        write(os, size);// Section size

        // Write section body
        char* buffer = new char[size];
        oss.rdbuf()->pubseekpos(0);// Return to first position
        oss.rdbuf()->sgetn(buffer, size);// Copy to buffer
        os.write(reinterpret_cast<const char*>(buffer), size);
        delete buffer;
    }

    void ProjectSerializer::deserializeGraphicsSystem(std::ifstream& is)
    {
        size_t sizeBytes;
        read(is, sizeBytes);

        unsigned numSections;
        read(is, numSections);

        unsigned curr = numSections;
        while(curr--)
        {
            std::string marker;
            read(is, marker);

            if(marker == "viewports")
            {
                // Read section size in bytes
                unsigned sectionSize;
                read(is, sectionSize);

                // Read number of viewports
                unsigned numViewports;
                read(is, numViewports);

                // Deserialize 
                std::vector<std::shared_ptr<Viewport>>& viewports = GraphicsManager::getViewportsNext();
                viewports.clear();
                for(unsigned i = 0; i < numViewports; i++)
                {
                    std::shared_ptr<Viewport> viewport;
                    viewport = std::make_shared<Viewport>(Viewport::CreateInfo{});
                    viewport->deserialize(is);
                    viewports.push_back(viewport);
                }
            }
        }
    }
}

