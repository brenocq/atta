//--------------------------------------------------
// Atta Graphics System
// simulationLayer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_SIMULATION_LAYER_H
#define ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_SIMULATION_LAYER_H
#include <atta/graphicsSystem/layers/layer.h>
#include <atta/memorySystem/allocatedObject.h>
#include <atta/graphicsSystem/pipeline.h>
#include <atta/graphicsSystem/renderers/fastRenderer.h>

namespace atta
{
	class SimulationLayer final : public Layer, public AllocatedObject<SimulationLayer, SID("GraphicsAllocator")>
	{
	public:
		SimulationLayer();

		void onAttach() override;
		void onDetach() override;
		void onRender() override;
		void onUIRender() override;
	private:
		std::shared_ptr<FastRenderer> _renderer;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_SIMULATION_LAYER_H
