//--------------------------------------------------
// Atta UI Module
// timeProfilerWindow.h
// Date: 2022-10-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_LAYERS_EDITOR_TOOLS_TIME_PROFILER_TIME_PROFILER_WINDOW_H
#define ATTA_UI_LAYERS_EDITOR_TOOLS_TIME_PROFILER_TIME_PROFILER_WINDOW_H

#include <atta/ui/layers/editor/window.h>
#include <atta/ui/layers/editor/tools/timeProfiler/components/tearDown.h>

namespace atta::ui {

class TimeProfilerWindow : public Window<TimeProfilerWindow> {
  private:
    TimeProfilerWindow();
    void renderImpl();

    friend Window<TimeProfilerWindow>;
  private:
    TearDown _tearDown;
};

} // namespace atta::ui

#endif // ATTA_UI_LAYERS_EDITOR_TOOLS_TIME_PROFILER_TIME_PROFILER_WINDOW_H
