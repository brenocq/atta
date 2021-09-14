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
        ImGuiIO& io = ImGui::GetIO();

		std::vector<std::shared_ptr<Viewport>> viewports = GraphicsManager::getViewports();
		static int activeViewport = 0;

		int i = -1;
		for(auto& viewport : viewports)
		{
			i++;
			// Render and resize
        	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
			ImGui::Begin(viewport->getSID().getString().c_str());
			{
				//----- Move camera -----//
				// Check started camera movement
				if(ImGui::IsMouseClicked(2) && ImGui::IsWindowHovered())
				{
					LOG_DEBUG("Editorlater", "($1) ACTIVE $0", i, viewport->getSID());
					activeViewport = i;
				}

				// Update camera (wheel pressed)
				if(activeViewport==i && ImGui::IsMouseDown(2))
				{
					vec3 position = viewport->getCamera()->getPosition();
					vec3 up = viewport->getCamera()->getUp();
					vec3 left = viewport->getCamera()->getLeft();

					position += up*io.MouseDelta.y*0.01f + -left*io.MouseDelta.x*0.01f;

					viewport->getCamera()->setPosition(position);
				}

				//----- Render to texture -----//
				ImVec2 size = ImVec2(viewport->getWidth(), viewport->getHeight());
				ImGui::Image(viewport->getImGuiTexture(), size, ImVec2(0, 0), ImVec2(1, 1));

				//----- Resize -----//
				ImVec2 windowSize = ImGui::GetWindowSize();
				if(windowSize.x != size.x || windowSize.y != size.y)
					viewport->resize((uint32_t)windowSize.x-10, (uint32_t)windowSize.y-30);
			}
			ImGui::End();
        	ImGui::PopStyleVar(1);

		}
	}
}
