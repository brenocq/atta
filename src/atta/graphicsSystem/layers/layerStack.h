//--------------------------------------------------
// Atta Graphics System
// layerStack.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_LAYERS_LAYER_STACK_H
#define ATTA_GRAPHICS_SYSTEM_LAYERS_LAYER_STACK_H
#include <atta/graphicsSystem/layers/layer.h>
#include <atta/graphicsSystem/layers/internal/uiLayer.h>

namespace atta
{
	class LayerStack
	{
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
		UILayer* _uiLayer;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_LAYERS_LAYER_STACK_H
