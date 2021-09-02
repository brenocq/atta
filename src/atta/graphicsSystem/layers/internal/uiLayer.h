//--------------------------------------------------
// Atta Graphics System
// uiLayer.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_UI_LAYER_H
#define ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_UI_LAYER_H
#include <atta/graphicsSystem/layers/layer.h>
#include <atta/memorySystem/allocatedObject.h>

namespace atta
{
	class UILayer final : public Layer, public AllocatedObject<UILayer, SID("GraphicsSystem")>
	{
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
}

#endif// ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_UI_LAYER_H
