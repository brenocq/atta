// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

namespace atta::ui {

class TearDown {
  public:
    TearDown();
    void render();

  private:
    void compute();
    void reset();

    /// Total execution time of each function
    struct FuncTime {
        StringId name;
        Profiler::Time time;
    };
    std::vector<FuncTime> _funcTime;
    size_t _lastRecordsSize;
    bool _exclusive;

    struct TimeInterval {
        Profiler::Time begin;
        Profiler::Time end;
    };
    /// List of time interval of nested functions
    /** Used when computing with exclusive intervals**/
    std::unordered_map<Profiler::ThreadId, std::vector<TimeInterval>> _nestedTime;
    /// Total recording time
    Profiler::Time _totalTime;
    /// Maximum Time in _funcTime
    Profiler::Time _maxTime;
};

} // namespace atta::ui
