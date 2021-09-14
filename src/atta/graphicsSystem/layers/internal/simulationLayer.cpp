//--------------------------------------------------
// Atta Graphics System
// simulationLayer.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/layers/internal/simulationLayer.h>

namespace atta
{

	SimulationLayer::SimulationLayer():
		Layer(StringId("GraphicsSimulationLayer"))
	{

	}

	void SimulationLayer::onAttach()
	{
		_renderer = std::make_shared<FastRenderer>();
	}

	void SimulationLayer::onDetach()
	{

	}

	void SimulationLayer::onRender()
	{
		//_renderer->render();
	}

	void SimulationLayer::onUIRender()
	{

	}
}
