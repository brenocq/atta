// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#include <atta/file/watchers/fileWatcher.h>

namespace atta::file {

class NullFileWatcher : public FileWatcher {
  public:
    NullFileWatcher() = default;
    virtual ~NullFileWatcher() = default;

    void addWatch(fs::path directory) override;
    void removeWatch(fs::path directory) override;

    void update() override;
};

} // namespace atta::file
