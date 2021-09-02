//--------------------------------------------------
// Atta Graphics System
// editorLayer.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_EDITOR_EDITOR_LAYER_H
#define ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_EDITOR_EDITOR_LAYER_H
#include <atta/graphicsSystem/layers/layer.h>
#include <atta/memorySystem/allocatedObject.h>
#include <atta/graphicsSystem/layers/internal/editor/menuBar.h>
#include <atta/graphicsSystem/layers/internal/editor/dockSpace.h>

namespace atta
{
	class EditorLayer final : public Layer, public AllocatedObject<EditorLayer, SID("Graphics")>
	{
	public:
		EditorLayer();

		void onAttach() override;
		void onDetach() override;
		void onRender() override;
		void onUIRender() override;
	private:
		MenuBar _menuBar;
		DockSpace _dockSpace;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_EDITOR_EDITOR_LAYER_H
