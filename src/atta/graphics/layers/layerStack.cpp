//--------------------------------------------------
// Atta Graphics Module
// layerStack.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/layers/layerStack.h>

namespace atta::graphics {
LayerStack::LayerStack() { _uiLayer = nullptr; }

LayerStack::~LayerStack() {
    for (int i = (int)_layers.size() - 1; i >= 0; i--) {
        Layer* layer = _layers[i];
        layer->onDetach();
        delete layer;
    }
}

void LayerStack::push(Layer* layer) {
    _layers.push_back(layer);
    layer->onAttach();

    if (layer->getSID() == SSID("GraphicsLayerUI"))
        _uiLayer = reinterpret_cast<ui::UILayer*>(layer);

    // LOG_VERBOSE("LayerStack", "Layer pushed: $0 (ptr:$1, size:$2)", layer->getSID(), layer, sizeof(layer));
}

void LayerStack::pop(Layer* layer) {
    auto it = std::find(_layers.begin(), _layers.end(), layer);
    if (it != _layers.end()) {
        layer->onDetach();
        _layers.erase(it);
    }
}

void LayerStack::render() {
    // Render layers
    for (auto layer : _layers)
        layer->onRender();

    // Render ui
    if (_uiLayer != nullptr) {
        _uiLayer->begin();
        for (auto layer : _layers)
            layer->onUIRender();
        _uiLayer->end();
    }
}
} // namespace atta::graphics
