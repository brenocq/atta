//--------------------------------------------------
// Atta Graphics System
// layerStack.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/layers/layerStack.h>

namespace atta
{
	LayerStack::LayerStack()
	{
		_uiLayer = nullptr;
	}

	LayerStack::~LayerStack()
	{
		for(int i = (int)_layers.size()-1; i >= 0; i--)
		{
			Layer* layer = _layers[i];
			layer->onDetach();
			delete layer;
		}
	}

	void LayerStack::push(Layer* layer)
	{
		_layers.push_back(layer);
		layer->onAttach();

		if(layer->getSID() == SID("UI"))
			_uiLayer = reinterpret_cast<UILayer*>(layer);

		LOG_VERBOSE("LayerStack", "Layer pushed: $0", layer->getSID());
	}

	void LayerStack::pop(Layer* layer)
	{
		auto it = std::find(_layers.begin(), _layers.end(), layer);
		if(it != _layers.end())
		{
			layer->onDetach();
			_layers.erase(it);
		}
	}


	void LayerStack::render()
	{
		// Render layers
		for(auto layer : _layers)
			layer->onRender();

		// Render ui
		if(_uiLayer != nullptr)
		{
			_uiLayer->begin();
			for(auto layer : _layers)
				layer->onUIRender();
			_uiLayer->end();
		}

	}
}
