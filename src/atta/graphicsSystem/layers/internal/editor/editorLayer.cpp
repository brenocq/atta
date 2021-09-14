//--------------------------------------------------
// Atta Graphics System
// editorLayer.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/layers/internal/editor/editorLayer.h>
#include <atta/graphicsSystem/graphicsManager.h>
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
		updateViewports();

		ImGui::Begin("Debug");
		ImGui::Text("Hello, down!");
		ImGui::End();
	}

	void EditorLayer::updateViewports()
	{
		std::vector<std::shared_ptr<Viewport>> viewports = GraphicsManager::getViewports();
		for(auto& viewport : viewports)
		{
        	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
			ImGui::Begin(viewport->getSID().getString().c_str());
			{
				ImVec2 size = ImVec2(viewport->getWidth(), viewport->getHeight());
				ImGui::Image(viewport->getImGuiTexture(), size, ImVec2(0, 0), ImVec2(1, 1));

				ImVec2 windowSize = ImGui::GetWindowSize();
				if(windowSize.x != size.x || windowSize.y != size.y)
					viewport->resize((uint32_t)windowSize.x-10, (uint32_t)windowSize.y-30);
			}
			ImGui::End();
        	ImGui::PopStyleVar(1);
		}
	}
}
