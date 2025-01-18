//--------------------------------------------------
// Atta UI Module
// timeProfilerWindow.h
// Date: 2022-10-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_EDITOR_TOOLS_TIME_PROFILER_TIME_PROFILER_WINDOW_H
#define ATTA_UI_EDITOR_TOOLS_TIME_PROFILER_TIME_PROFILER_WINDOW_H

#include <atta/ui/windows/window.h>
#include <atta/ui/editor/tools/timeProfiler/components/tearDown.h>
#include <atta/ui/editor/tools/timeProfiler/components/flameGraph.h>
#include <atta/ui/editor/tools/timeProfiler/components/timeline.h>

namespace atta::ui {

class TimeProfilerWindow : public Window<TimeProfilerWindow> {
  private:
    TimeProfilerWindow();
    void renderImpl();

    friend Window<TimeProfilerWindow>;
  private:
    TearDown _tearDown;
    FlameGraph _flameGraph;
    Timeline _timeline;
};

} // namespace atta::ui

#endif // ATTA_UI_EDITOR_TOOLS_TIME_PROFILER_TIME_PROFILER_WINDOW_H
