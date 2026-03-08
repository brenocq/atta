// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/file/watchers/nullFileWatcher.h>

namespace atta::file {

void NullFileWatcher::addWatch(fs::path directory) {
    LOG_WARN("file::NullFileWatcher", "Watching files was not implemented to this operating system. Can't watch [w]$0", directory);
    FileWatcher::addWatch(directory);
}

void NullFileWatcher::removeWatch(fs::path directory) { FileWatcher::removeWatch(directory); }

void NullFileWatcher::update() {}

} // namespace atta::file
