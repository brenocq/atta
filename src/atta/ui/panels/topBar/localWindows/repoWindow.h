// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/component/interface.h>

namespace atta::ui {

class RepoWindow {
  public:
    RepoWindow(fs::path repoPath);

    void render();

    fs::path getRepoPath() { return _repoPath; }
    bool getShouldClose() { return _shouldClose; }

  private:
    struct ReadmeMetadata {
        std::string title;
        std::string description;
        std::string image;
    };

    struct ProjectInfo {
        ReadmeMetadata metadata;
        fs::path attaFile;
    };

    /// Search for projects recursivelly and populate _projects
    void findProjects();
    /// Parse README.md file and extract metadata
    ReadmeMetadata parseReadmeMetadata(fs::path readme);

    fs::path _repoPath;
    std::string _title;
    std::string _description;
    std::vector<ProjectInfo> _projects;

    bool _shouldClose;
};

} // namespace atta::ui
