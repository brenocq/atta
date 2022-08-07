//--------------------------------------------------
// Atta Graphics Module
// layerStack.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_LAYERS_LAYER_STACK_H
#define ATTA_GRAPHICS_LAYERS_LAYER_STACK_H

#include <atta/graphics/layers/layer.h>
#include <atta/ui/layers/uiLayer.h>

namespace atta::graphics {

class LayerStack {
  public:
    LayerStack();
    ~LayerStack();

    void push(Layer* layer);
    void pop(Layer* layer);

    size_t size() const { return _layers.size(); }

    std::vector<Layer*>::iterator begin() { return _layers.begin(); }
    std::vector<Layer*>::iterator end() { return _layers.end(); }

    void render();

  private:
    std::vector<Layer*> _layers;
    ui::UILayer* _uiLayer;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_LAYERS_LAYER_STACK_H
