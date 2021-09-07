//--------------------------------------------------
// Atta Graphics System
// editorLayer.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/layers/internal/editor/editorLayer.h>
#include <imgui_internal.h>

namespace atta
{
	EditorLayer::EditorLayer():
		Layer(StringId("GraphicsEditorLayer"))
	{
	}

	void EditorLayer::onAttach()
	{
	}

	void EditorLayer::onDetach()
	{
	}

	void EditorLayer::onRender()
	{
	}

	void EditorLayer::onUIRender()
	{
		_dockSpace.render();
		_menuBar.render();

		bool demo = true;
		ImGui::ShowDemoWindow(&demo);

		_scenePanel.render();

		ImGui::Begin("Debug");
		ImGui::Text("Hello, down!");
		ImGui::End();
	}
}
