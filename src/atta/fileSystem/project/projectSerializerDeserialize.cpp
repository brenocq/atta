//--------------------------------------------------
// Atta File System
// projectSerializerDeserialize.cpp
// Date: 2022-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta
{
    //ProjectSerializer::Header ProjectSerializer::deserializeHeader(std::ifstream& is)
    //{
    //    //std::string marker;
    //    //Header header;

    //    //while(true)
    //    //{
    //    //    read(is, marker);
    //    //    if(marker == "hend")
    //    //    {
    //    //        return header;
    //    //    }
    //    //    else if(marker == "atta")
    //    //    {
    //    //        read(is, header.version);
    //    //    }
    //    //    else if(marker == "proj")
    //    //    {
    //    //        read(is, header.projectName);
    //    //        read(is, header.saveCounter);
    //    //    }
    //    //    else
    //    //    {
    //    //        LOG_WARN("ProjectSerializer", "Unknown marker found at the header [w]$0[]", marker);
    //    //        return header;
    //    //    }
    //    //}
    //    //return header;
    //}

    //void ProjectSerializer::deserializeConfig(std::ifstream& is)
    //{
    //    //std::string marker;
    //    //while(true)
    //    //{
    //    //    read(is, marker);
    //    //    if(marker == "dt")
    //    //    {
    //    //        float dt;
    //    //        read(is, dt);
    //    //        Config::setDt(dt);
    //    //    }
    //    //    else if(marker == "cend")
    //    //    {
    //    //        return;
    //    //    }
    //    //    else
    //    //    {
    //    //        LOG_WARN("ProjectSerializer", "Unknown marker found at the config [w]$0[]", marker);
    //    //    }
    //    //}
    //}

    //void ProjectSerializer::deserializeComponentSystem(std::ifstream& is)
    //{
    //    // Read section size in bytes
    //    //size_t sizeBytes;
    //    //read(is, sizeBytes);
    //    //int endSectionPos = int(is.tellg()) + sizeBytes;// Used to validate position at the end

    //    //// Read section
    //    //std::string marker;
    //    //uint32_t numComponents = 0;

    //    //// Read entity ids
    //    //{
    //    //    read(is, marker);
    //    //    if(marker != "id")
    //    //    {
    //    //        LOG_WARN("ProjectSerializer", "First component marker must be [w]id[], but it is [w]$0[]. Could not load atta file", marker);
    //    //        return;
    //    //    }

    //    //    uint64_t numIds;
    //    //    read(is, numIds);
    //    //    for(uint64_t i = 0; i < numIds; i++)
    //    //    {
    //    //        EntityId id;
    //    //        read(is, id);

    //    //        EntityId res = ComponentManager::createEntity(id);
    //    //        if(res != id)
    //    //            LOG_WARN("ProjectSerializer","Could not create entity with id $0", id);
    //    //    }
    //    //}

    //    //// Read number of components
    //    //read(is, numComponents);

    //    //for(uint32_t i = 0; i < numComponents; i++)
    //    //{
    //    //    uint32_t componentSectionSize;// Can be used to skip the component data if it is an unknown component

    //    //    // Read component name
    //    //    read(is, marker);
    //    //    read(is, componentSectionSize);
    //    //    ComponentRegistry* compReg = nullptr;
    //    //    int componentSectionEndPos = int(is.tellg()) + componentSectionSize;

    //    //    // Find correct component registry
    //    //    for(auto componentRegistry : ComponentManager::getComponentRegistries())
    //    //    {
    //    //        if(componentRegistry->getDescription().name == marker)
    //    //        {
    //    //            compReg = componentRegistry;
    //    //            break;
    //    //        }
    //    //    }

    //    //    // If not found the component, skip this section
    //    //    if(compReg == nullptr)
    //    //    {
    //    //        is.ignore(componentSectionEndPos);
    //    //        continue;
    //    //    }

    //    //    // Deserialize 
    //    //    while(int(is.tellg()) < componentSectionEndPos)
    //    //    {
    //    //        EntityId eid;
    //    //        read(is, eid);
    //    //        Component* component = ComponentManager::addEntityComponentById(compReg->getId(), eid);
    //    //        compReg->deserialize(is, component);
    //    //    }

    //    //    // Check finished at right position
    //    //    if(int(is.tellg()) != componentSectionEndPos)
    //    //    {
    //    //        LOG_WARN("ProjectSerializer", "Expected position ([w]$0[]) and actual position ([w]$1[])does not match for [*w]component $2 section[]. Some data may have been incorrectly initialized for this component", (int)is.tellg(), componentSectionEndPos, compReg->getDescription().name);
    //    //        is.seekg(componentSectionEndPos, std::ios_base::beg);
    //    //    }
    //    //}

    //    //if(int(is.tellg()) != endSectionPos)
    //    //{
    //    //    LOG_WARN("ProjectSerializer", "Expected position ([w]$0[]) and actual position ([w]$1[])does not match for the [*w]component system section[]. Some data may have been incorrectly initialized", (int)is.tellg(), endSectionPos);
    //    //    is.seekg(endSectionPos, std::ios_base::beg);
    //    //}
    //}

    //void ProjectSerializer::deserializeGraphicsSystem(std::ifstream& is)
    //{
    //    //size_t sizeBytes;
    //    //read(is, sizeBytes);
    //    //int sectionEndPos = int(is.tellg()) + sizeBytes;// Used to validate position at the end

    //    //unsigned numSections;
    //    //read(is, numSections);

    //    //unsigned curr = numSections;
    //    //while(curr--)
    //    //{
    //    //    std::string marker;
    //    //    read(is, marker);

    //    //    if(marker == "viewports")
    //    //    {
    //    //        // Read section size in bytes
    //    //        unsigned sectionSize;
    //    //        read(is, sectionSize);

    //    //        // Read number of viewports
    //    //        unsigned numViewports;
    //    //        read(is, numViewports);

    //    //        // Deserialize 
    //    //        GraphicsManager::clearViewports();
    //    //        for(unsigned i = 0; i < numViewports; i++)
    //    //        {
    //    //            std::shared_ptr<Viewport> viewport;
    //    //            viewport = std::make_shared<Viewport>(Viewport::CreateInfo{});
    //    //            viewport->deserialize(is);
    //    //            GraphicsManager::addViewport(viewport);
    //    //        }
    //    //    }
    //    //}

    //    //if(int(is.tellg()) != sectionEndPos)
    //    //{
    //    //    LOG_WARN("ProjectSerializer", "Expected position ([w]$0[]) and actual position ([w]$1[])does not match for the [*w]graphics system section[]. Some data may have been incorrectly initialized", (int)is.tellg(), sectionEndPos);
    //    //    is.seekg(sectionEndPos, std::ios_base::beg);
    //    //}
    //}
}

