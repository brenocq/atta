//--------------------------------------------------
// Atta File Module
// interface.cpp
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/interface.h>
#include <atta/file/manager.h>

namespace atta::file {

void startUp() { Manager::getInstance().startUpImpl(); }
void shutDown() { Manager::getInstance().shutDownImpl(); }

// Project
bool openProject(fs::path projectFile) { return Manager::getInstance().openProjectImpl(projectFile); }
bool createProject(fs::path projectFile) { return Manager::getInstance().createProjectImpl(projectFile); }
void saveProject() { Manager::getInstance().saveProjectImpl(); }
void closeProject() { Manager::getInstance().closeProjectImpl(); }
bool isProjectOpen() { return Manager::getInstance().isProjectOpenImpl(); }
std::shared_ptr<Project> getProject() { return Manager::getInstance().getProjectImpl(); }

fs::path solveResourcePath(fs::path relativePath, bool mustExist) { return Manager::getInstance().solveResourcePathImpl(relativePath, mustExist); }

std::vector<fs::path> getResourcePaths() { return Manager::getInstance().getResourcePathsImpl(); }

fs::path getBuildPath() { return Manager::getInstance().getBuildPathImpl(); }

std::vector<fs::path> getDirectoryFilesRecursive(const fs::path& directory) {
    return Manager::getInstance().getDirectoryFilesRecursiveImpl(directory);
}

fs::path getDefaultProjectFolder() { return Manager::getInstance()._defaultProjectFolder; }

void update() {
    PROFILE();
    Manager::getInstance().updateImpl();
};

} // namespace atta::file
