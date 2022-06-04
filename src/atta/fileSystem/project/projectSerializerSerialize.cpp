//--------------------------------------------------
// Atta File System
// projectSerializerSerialize.cpp
// Date: 2022-06-02
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta
{
    void ProjectSerializer::serializeHeader(Section& section)
    {
        Section& header = section["header"];
        header["version"] = std::vector<uint16_t>{ ATTA_VERSION_MAJOR, ATTA_VERSION_MINOR, ATTA_VERSION_PATCH, ATTA_VERSION_TWEAK };
        header["projectName"] = std::string(_project->getName());
    }

    //void ProjectSerializer::serializeConfig(std::ofstream& os)
    //{
    //    //write(os, "config");

    //    //// Dt
    //    //write(os, "dt");
    //    //write(os, Config::getDt());

    //    //write(os, "cend");
    //}

    //void ProjectSerializer::serializeComponentSystem(std::ofstream& os)
    //{
    //    //std::stringstream oss;
    //    //std::basic_ostream<char>::pos_type posBefore = oss.tellp();

    //    ////---------- Write to temporary buffer ----------//
    //    //// Serialize entity ids
    //    //std::vector<EntityId> entities = ComponentManager::getNoCloneView();
    //    //write(oss, "id");// Entity id marker
    //    //write<uint64_t>(oss, entities.size());
    //    //for(EntityId entity : entities)
    //    //    write(oss, entity);

    //    //// Serialize number of components
    //    //write<uint32_t>(oss, ComponentManager::getComponentRegistries().size());

    //    //// Serialize components
    //    //for(auto compReg : ComponentManager::getComponentRegistries())
    //    //{
    //    //    // Write name
    //    //    write(oss, compReg->getDescription().name);

    //    //    // Get all entities that have this component
    //    //    std::vector<std::pair<EntityId,Component*>> pairs;
    //    //    for(auto entity : entities)
    //    //    {
    //    //        Component* comp = ComponentManager::getEntityComponentById(compReg->getId(), entity);
    //    //        if(comp != nullptr)
    //    //            pairs.push_back(std::make_pair(entity, comp));
    //    //    }

    //    //    // Write size in bytes of this next section (can be useful if the component is unknown and can't be deserialized)
    //    //    unsigned totalSectionSize = 0;
    //    //    for(auto compPair : pairs)
    //    //        totalSectionSize += compReg->getSerializedSize(compPair.second) + sizeof(EntityId);
    //    //    write<uint32_t>(oss, totalSectionSize);

    //    //    std::basic_ostream<char>::pos_type posBef = oss.tellp();
    //    //    for(auto compPair : pairs)
    //    //    {
    //    //        // TODO Open vector
    //    //        write(oss, compPair.first);
    //    //        compReg->serialize(oss, compPair.second);
    //    //    }
    //    //    ASSERT(oss.tellp()-posBef == totalSectionSize, "Serialized section size and calculated section size does not match");
    //    //}

    //    ////---------- Flush buffer to file ----------//
    //    //// Write section header
    //    //size_t size = (int)oss.tellp() - posBefore;
    //    //write(os, "comp");// Section title
    //    //write(os, size);// Section size

    //    //// Write section body
    //    //char* buffer = new char[size];
    //    //oss.rdbuf()->pubseekpos(0);// Return to first position
    //    //oss.rdbuf()->sgetn(buffer, size);// Copy to buffer
    //    //os.write(reinterpret_cast<const char*>(buffer), size);
    //    //delete[] buffer;
    //}

    //void ProjectSerializer::serializeGraphicsSystem(std::ofstream& os)
    //{
    //    //std::stringstream oss;
    //    //std::basic_ostream<char>::pos_type posBefore = oss.tellp();

    //    ////---------- Write to temporary buffer ----------//
    //    //// Number of subsections
    //    //write<uint32_t>(oss, 1);
    //    ////----- Viewport subsection -----//
    //    //write(oss, "viewports");

    //    //std::vector<std::shared_ptr<Viewport>> viewports = GraphicsManager::getViewports();
    //    //unsigned sectionSize = 0;
    //    //for(auto viewport : viewports)
    //    //    sectionSize += viewport->getSerializedSize();
    //    //// Serialize section size in bytes
    //    //write<uint32_t>(oss, sectionSize);

    //    //// Serialize number of viewports
    //    //write<uint32_t>(oss, viewports.size());

    //    //// Serialize 
    //    //for(auto viewport : viewports)
    //    //    viewport->serialize(oss);

    //    ////---------- Flush buffer to file ----------//
    //    //// Write section header
    //    //size_t size = (int)oss.tellp() - posBefore;
    //    //write(os, "gfx");// Section title
    //    //write(os, size);// Section size

    //    //// Write section body
    //    //char* buffer = new char[size];
    //    //oss.rdbuf()->pubseekpos(0);// Return to first position
    //    //oss.rdbuf()->sgetn(buffer, size);// Copy to buffer
    //    //os.write(reinterpret_cast<const char*>(buffer), size);
    //    //delete[] buffer;
    //}
}

