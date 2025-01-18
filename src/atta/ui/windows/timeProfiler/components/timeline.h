//--------------------------------------------------
// Atta UI Module
// timeline.h
// Date: 2022-10-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_WINDOWS_TIME_PROFILER_COMPONENTS_TIMELINE_H
#define ATTA_UI_WINDOWS_TIME_PROFILER_COMPONENTS_TIMELINE_H

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

#endif // ATTA_UI_WINDOWS_TIME_PROFILER_COMPONENTS_TIMELINE_H
