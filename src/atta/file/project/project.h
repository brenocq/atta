//--------------------------------------------------
// Atta File Module
// project.h
// Date: 2021-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_PROJECT_PROJECT_H
#define ATTA_FILE_PROJECT_PROJECT_H
#include <atta/file/project/project.h>

namespace atta::file {

class Manager;
class Project final {
  public:
    Project(fs::path file);

    std::string getName() { return _name; }
    fs::path getFile() const { return _file; }
    fs::path getDirectory() const { return _directory; }

    fs::path getBuildDirectory();
    fs::path getSnapshotDirectory();
    std::vector<fs::path> getResourceRootPaths() const { return _resourceRootPaths; }

  private:
    fs::path solveResourcePath(fs::path relativePath, bool mustExist = true);

    std::string _name;

    fs::path _file; // .atta file
    fs::path _directory;

    std::vector<fs::path> _resourceRootPaths;

    friend Manager;
};

} // namespace atta::file

#endif // ATTA_FILE_PROJECT_PROJECT_H
