//--------------------------------------------------
// Atta File Module
// interface.h
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_INTERFACE_H
#define ATTA_FILE_INTERFACE_H

#include <atta/component/base.h>
#include <atta/component/components/component.h>
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

//----- Custom component serialization -----//
// using SerializeFunc = std::function<void(Section&, cmp::Component*)>;
// using DeserializeFunc = std::function<void(const Section&, cmp::Component*)>;
// using RenderFunc = std::function<void(cmp::Component*)>;
// void registerComponentIO(cmp::ComponentId cid, SerializeFunc serialize, DeserializeFunc deserialize);

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

#endif // ATTA_FILE_INTERFACE_H
