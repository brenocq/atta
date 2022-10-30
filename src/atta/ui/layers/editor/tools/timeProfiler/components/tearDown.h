//--------------------------------------------------
// Atta UI Module
// tearDown.h
// Date: 2022-10-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_LAYERS_EDITOR_TOOLS_TIME_PROFILER_COMPONENTS_TEAR_DOWN_H
#define ATTA_UI_LAYERS_EDITOR_TOOLS_TIME_PROFILER_COMPONENTS_TEAR_DOWN_H

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

#endif // ATTA_UI_LAYERS_EDITOR_TOOLS_TIME_PROFILER_COMPONENTS_TEAR_DOWN_H
