//--------------------------------------------------
// Atta Graphics System
// scenePanel.cpp
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/layers/internal/editor/scenePanel.h>
#include <atta/componentSystem/components/components.h>
#include <imgui.h>

namespace atta
{
	ScenePanel::ScenePanel():
		_selected(0), _someSelected(false)
	{
	}

	void ScenePanel::render()
	{
		ImGui::Begin("Scene");
		{
			renderTree();
        	ImGui::Separator();
			renderComponents();
		}
		ImGui::End();
	}

	void ScenePanel::renderTree()
	{
		if(ImGui::Button("Create"))
			ComponentManager::createEntity();

		static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | 
			ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_NoBordersInBody;
		if(ImGui::BeginTable("Scene", 1, flags, ImVec2(0, 250.0f)))
		{
			// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
			//ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed, 30.0f);
			ImGui::TableHeadersRow();

			std::vector<EntityId> entities = ComponentManager::getEntities();

			for(EntityId entity : entities)
			{
				std::string name = "<Object "+std::to_string(entity)+">";
				NameComponent* n = ComponentManager::getEntityComponent<NameComponent>(entity);
				if(n != nullptr) name = n->name;

				ImGui::TableNextRow();
				ImGui::TableNextColumn();

                ImGui::Selectable(name.c_str(), _selected == entity);
                if(ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
					_selected = entity;

                if(ImGui::BeginPopupContextItem()) // <-- use last item id as popup id
                {
                    ImGui::Text("Delete %lu", entity);
                    ImGui::Text("Add component");
                    if (ImGui::Button("Close"))
                        ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                }
			}
			ImGui::EndTable();
		}
	}

	void ScenePanel::renderComponents()
	{
		ImGui::Text("Components (ID:%lu)", _selected);
		ImGui::SameLine();
		if(ImGui::Button("Add"))
            ImGui::OpenPopup("Scene_ComponentAdd");

		if(ImGui::BeginPopup("Scene_ComponentAdd"))
		{
			if(ImGui::Selectable("Name"))
			{
				NameComponent* name = ComponentManager::addEntityComponent<NameComponent>(_selected);
				if(name != nullptr)
					name->name[0] = '\0';
			}

			if(ImGui::Selectable("Transform"))
			{
				TransformComponent* transform = ComponentManager::addEntityComponent<TransformComponent>(_selected);
				if(transform != nullptr)
					transform->position = {0};
			}
			ImGui::EndPopup();
		}

		NameComponent* name = ComponentManager::getEntityComponent<NameComponent>(_selected);
		if(name != nullptr)
			if(ImGui::CollapsingHeader("Name", ImGuiTreeNodeFlags_None))
            	ImGui::InputText("Name", name->name, sizeof(NameComponent::name));

		TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(_selected);
		if(transform != nullptr)
			if(ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_None))
			{
				ImGui::Text("Position");


				{
					ImVec4 color = ImVec4(0.6f, 0.2f, 0.2f, 1.0f);
					ImGui::PushStyleColor(ImGuiCol_Button, color);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
					if(ImGui::Button("X"));
					ImGui::PopStyleColor(3);
					ImGui::SameLine();
					ImGui::DragFloat("##Transform_X", &transform->position.x, 0.005f, -FLT_MAX, +FLT_MAX, "%.6f", ImGuiSliderFlags_None);
				}

				{
					ImVec4 color = ImVec4(0.2f, 0.6f, 0.2f, 1.0f);
					ImGui::PushStyleColor(ImGuiCol_Button, color);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
					if(ImGui::Button("Y"));
					ImGui::PopStyleColor(3);
					ImGui::SameLine();
					ImGui::DragFloat("##TransformY", &transform->position.y, 0.005f, -FLT_MAX, +FLT_MAX, "%.6f", ImGuiSliderFlags_None);
				}

				{
					ImVec4 color = ImVec4(0.2f, 0.2f, 0.6f, 1.0f);
					ImGui::PushStyleColor(ImGuiCol_Button, color);
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);
					if(ImGui::Button("Z"));
					ImGui::PopStyleColor(3);
					ImGui::SameLine();
					ImGui::DragFloat("##TransformZ", &transform->position.z, 0.005f, -FLT_MAX, +FLT_MAX, "%.6f", ImGuiSliderFlags_None);
				}
			}
	}

	bool ScenePanel::getSelected(EntityId& eid)
	{
		eid = _selected;
		return _someSelected;
	}
}
