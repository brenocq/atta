//--------------------------------------------------
// Atta UI Module
// uiLayer.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_LAYERS_UI_LAYER_H
#define ATTA_UI_LAYERS_UI_LAYER_H

#include <atta/graphics/layers/layer.h>
#include <atta/memory/allocatedObject.h>

namespace atta::ui {

class UILayer final : public graphics::Layer, public memory::AllocatedObject<UILayer, SID("GraphicsAllocator")> {
  public:
    UILayer();

    void onAttach() override;
    void onDetach() override;
    void onRender() override;
    void onUIRender() override;

    void begin();
    void end();

  private:
    void initOpenGL();
    void initVulkan();

    void setTheme();
};

} // namespace atta::ui

#endif // ATTA_UI_LAYERS_UI_LAYER_H
