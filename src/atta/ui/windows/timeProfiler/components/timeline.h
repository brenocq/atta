// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

namespace atta::ui {

class Timeline {
  public:
    void render();

  private:
    void compute();
    void reset();

    /// Records sorted ascending with begin
    std::unordered_map<Profiler::ThreadId, std::vector<Profiler::Record>> _sortedRecords;
    std::unordered_map<Profiler::ThreadId, std::stack<Profiler::Record>> _currStack;
    std::unordered_map<Profiler::ThreadId, size_t> _maxStackSize;
    size_t _lastRecordsSize;
};

} // namespace atta::ui
