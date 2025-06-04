//--------------------------------------------------
// Atta File Module
// manager.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_FILE_MANAGER_H
#define ATTA_FILE_FILE_MANAGER_H
#include <atta/event/interface.h>
#include <atta/file/interface.h>
#include <atta/file/project/projectSerializer.h>
#include <atta/file/watchers/fileWatcher.h>

namespace atta::file {

class Manager final {
  public:
    static Manager& getInstance();

    friend void startUp();
    friend void shutDown();
    friend bool openProject(fs::path projectFile);
    friend bool createProject(fs::path projectFile);
    friend void saveProject();
    friend void closeProject();
    friend bool isProjectOpen();
    friend std::shared_ptr<Project> getProject();

    friend void registerComponentIO(cmp::ComponentId cid, const SerializeFunc& serialize, const DeserializeFunc& deserialize);
    friend void getComponentIO(cmp::ComponentId cid, std::optional<SerializeFunc>& serialize, std::optional<DeserializeFunc>& deserialize);

    friend fs::path solveResourcePath(fs::path relativePath, bool mustExist);
    friend std::vector<fs::path> getResourcePaths();
    friend fs::path getBuildPath();
    friend std::vector<fs::path> getDirectoryFilesRecursive(const fs::path& directory);
    friend fs::path getDefaultProjectFolder();
    friend void update();

  private:
    void startUpImpl();
    void shutDownImpl();

    bool openProjectImpl(fs::path projectFile);
    bool createProjectImpl(fs::path projectFile);
    void saveProjectImpl();
    void closeProjectImpl();
    bool isProjectOpenImpl() const;
    std::shared_ptr<Project> getProjectImpl() const { return _project; }

    void registerCustomComponentIOs();

    void registerComponentIOImpl(cmp::ComponentId cid, const SerializeFunc& serialize, const DeserializeFunc& deserialize);
    void getComponentIOImpl(cmp::ComponentId cid, std::optional<SerializeFunc>& serialize, std::optional<DeserializeFunc>& deserialize) const;

    fs::path solveResourcePathImpl(fs::path relativePath, bool mustExist);
    std::vector<fs::path> getResourcePathsImpl() const;
    fs::path getBuildPathImpl() const;
    std::vector<fs::path> getDirectoryFilesRecursiveImpl(const fs::path& directory);

    // Handle events
    void onSimulationStateChange(event::Event& event);

    // TODO remove
    void updateImpl();

    std::shared_ptr<FileWatcher> _fileWatcher;
    std::shared_ptr<Project> _project;
    std::shared_ptr<ProjectSerializer> _projectSerializer;
    bool _simulationRunning;
    fs::path _defaultProjectFolder; ///< Default folder to clone published projects and save projects

    std::map<cmp::ComponentId, SerializeFunc> _componentSerialize;     // Custom component serialization
    std::map<cmp::ComponentId, DeserializeFunc> _componentDeserialize; // Custom component deserialization
};

} // namespace atta::file

#endif // ATTA_FILE_FILE_MANAGER_H
