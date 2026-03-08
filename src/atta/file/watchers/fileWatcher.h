// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

namespace atta::file {

class FileWatcher {
  public:
    FileWatcher() = default;
    virtual ~FileWatcher() = default;

    virtual void addWatch(fs::path directory);
    virtual void removeWatch(fs::path directory);
    virtual void update() = 0;

  protected:
    std::map<fs::path, int> _pathsToWatch;
};

} // namespace atta::file
