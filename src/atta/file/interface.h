//--------------------------------------------------
// Atta File Module
// interface.h
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_INTERFACE_H
#define ATTA_FILE_INTERFACE_H

#include <atta/component/interface.h>
#include <atta/file/project/project.h>
#include <atta/file/serializer/section.h>

namespace atta::file {

void startUp();
void shutDown();

//----- Project -----//
bool openProject(fs::path projectFile);
bool createProject(fs::path projectFile);
void saveProject();
void closeProject();
bool isProjectOpen();
std::shared_ptr<Project> getProject();

//----- Custom component (de)serialization -----//
using SerializeFunc = std::function<void(Section&, cmp::Component*)>;
using DeserializeFunc = std::function<void(const Section&, cmp::Entity)>;
template <typename T>
void registerComponentIO(const SerializeFunc& serialize, const DeserializeFunc& deserialize);
void registerComponentIO(cmp::ComponentId cid, const SerializeFunc& serialize, const DeserializeFunc& deserialize);

template <typename T>
void getComponentIO(std::optional<SerializeFunc>& serialize, std::optional<DeserializeFunc>& deserialize);
void getComponentIO(cmp::ComponentId cid, std::optional<SerializeFunc>& serialize, std::optional<DeserializeFunc>& deserialize);

//----- Path -----//
// Receives a relative resource path and searches the registered directories for that file
// By default searches on the <ATTA_DIR>/resources and <PROJECT_DIR>/resources directories
// The return is the absolute resource path
fs::path solveResourcePath(fs::path relativePath, bool mustExist = true);
std::vector<fs::path> getResourcePaths();
fs::path getBuildPath();
std::vector<fs::path> getDirectoryFilesRecursive(const fs::path& directory);
fs::path getDefaultProjectFolder();

void update();

} // namespace atta::file

#include <atta/file/interface.inl>

#endif // ATTA_FILE_INTERFACE_H
