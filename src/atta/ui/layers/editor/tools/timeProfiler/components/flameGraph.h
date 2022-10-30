//--------------------------------------------------
// Atta UI Module
// flameGraph.h
// Date: 2022-10-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_LAYERS_EDITOR_TOOLS_TIME_PROFILER_COMPONENTS_FLAME_GRAPH_H
#define ATTA_UI_LAYERS_EDITOR_TOOLS_TIME_PROFILER_COMPONENTS_FLAME_GRAPH_H

namespace atta::ui {

class FlameGraph {
  public:
    void render();

  private:
    void compute();
    void reset();

    struct FlameBar {
        StringId name;
        Profiler::Time time;
        std::vector<FlameBar> nested;
    };
    FlameBar _flameBar;
    size_t _lastRecordsSize;
};

} // namespace atta::ui

#endif // ATTA_UI_LAYERS_EDITOR_TOOLS_TIME_PROFILER_COMPONENTS_FLAME_GRAPH_H
