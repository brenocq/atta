// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#include <atta/file/watchers/fileWatcher.h>

namespace atta::file {

void FileWatcher::addWatch(fs::path directory) { _pathsToWatch[directory] = -1; }

void FileWatcher::removeWatch(fs::path directory) { _pathsToWatch.erase(directory); }

} // namespace atta::file
