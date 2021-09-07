//--------------------------------------------------
// Atta Graphics System
// layer2D.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_LAYER_2D_H
#define ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_LAYER_2D_H
#include <atta/graphicsSystem/layers/layer.h>
#include <atta/memorySystem/allocatedObject.h>

namespace atta
{
	class Layer2D final : public Layer, public AllocatedObject<Layer2D, SID("GraphicsAllocator")>
	{
	public:
		Layer2D();

		void onAttach() override;
		void onDetach() override;
		void onRender() override;
		void onUIRender() override;
	private:
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_LAYER_2D_H
