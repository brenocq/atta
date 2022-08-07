//--------------------------------------------------
// Atta UI Module
// uiLayer.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_LAYERS_INTERNAL_UI_LAYER_H
#define ATTA_GRAPHICS_LAYERS_INTERNAL_UI_LAYER_H
#include <atta/graphics/layers/layer.h>
#include <atta/memory/allocatedObject.h>

namespace atta::ui {
class UILayer final : public Layer, public AllocatedObject<UILayer, SID("GraphicsAllocator")> {
  public:
    UILayer();

    void onAttach() override;
    void onDetach() override;
    void onRender() override;
    void onUIRender() override;

    void begin();
    void end();

  private:
    void setTheme();
};
} // namespace atta::ui

#endif // ATTA_GRAPHICS_LAYERS_INTERNAL_UI_LAYER_H
