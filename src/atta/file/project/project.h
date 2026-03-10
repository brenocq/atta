// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

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
    friend class ProjectSerializer;
};

} // namespace atta::file
