//--------------------------------------------------
// Atta Graphics Module
// layer.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_LAYERS_LAYER_H
#define ATTA_GRAPHICS_LAYERS_LAYER_H

#include <atta/utils/stringId.h>

namespace atta::graphics {

class Layer {
  public:
    Layer(StringId sid = StringId("Layer"));
    virtual ~Layer() = default;

    virtual void onAttach() = 0;
    virtual void onDetach() = 0;
    virtual void onRender() = 0;
    virtual void onUIRender() = 0;

    StringId getSID() { return _sid; }
    std::string getName() { return _sid.getString(); }

  private:
    StringId _sid;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_LAYERS_LAYER_H
