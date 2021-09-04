//--------------------------------------------------
// Atta Graphics System
// sceneTree.cpp
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/layers/internal/editor/sceneTree.h>
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/components.h>
#include <imgui.h>

namespace atta
{
	void SceneTree::render()
	{
		if(ImGui::Button("Create"))
			ComponentManager::createEntity();

		static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ContextMenuInBody;
		if(ImGui::BeginTable("Scene", 2, flags, ImVec2(0, 250.0f)))
		{
			// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
			ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 30.0f);
			ImGui::TableHeadersRow();

			std::vector<EntityId> entities = ComponentManager::getEntities();

			for(EntityId entity : entities)
			{
				std::string name = "No name component";
				Name* n = ComponentManager::getEntityComponent<Name>(entity);
				if(n != nullptr) name = n->name;

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
                ImGui::Selectable(name.c_str());
                if(ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
                {
                    ImGui::Text("Delete");
                    ImGui::Text("Add component");
                    if (ImGui::Button("Close"))
                        ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                }

				ImGui::TableNextColumn();
				ImGui::Text("%lu", entity);

			}
			ImGui::EndTable();
		}

        ImGui::Separator();
	}
}

