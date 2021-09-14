//--------------------------------------------------
// Atta Graphics System
// dockSpace.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/layers/internal/editor/dockSpace.h>
#include <imgui_internal.h>

namespace atta
{
	void DockSpace::render()
	{
		//----- Create DockSpace -----//
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground;

		bool open = true;
    	ImGui::Begin("EditorDockSpace", &open, window_flags);
        ImGui::PopStyleVar(3);

		ImGuiIO& io = ImGui::GetIO();
		if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("EditorDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

			//----- Clear previous layout -----//
			static auto firstTime = true;
			if(firstTime)
			{
				firstTime = false;

				ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
				ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

				auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.20f, nullptr, &dockspace_id);
				auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.20f, nullptr, &dockspace_id);

				// we now dock our windows into the docking node we made above
				ImGui::DockBuilderDockWindow("Main Viewport", dockspace_id);
				ImGui::DockBuilderDockWindow("Debug", dock_id_down);
				ImGui::DockBuilderDockWindow("Scene", dock_id_right);
				ImGui::DockBuilderFinish(dockspace_id);
			}
		}
		ImGui::End();
	}
}
