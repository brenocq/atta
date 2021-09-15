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

			_someSelected = false;
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

				// If entity is still selected
				if(entity == _selected)
					_someSelected = true;
			}
			ImGui::EndTable();
		}
	}

	void ScenePanel::renderComponents()
	{
		if(!_someSelected)
			return;

		ImGui::Text("Components (ID:%lu)", _selected);
		ImGui::SameLine();
		if(ImGui::Button("Add"))
            ImGui::OpenPopup("Scene_ComponentAdd");

		if(ImGui::BeginPopup("Scene_ComponentAdd"))
		{
			if(ImGui::Selectable("Name##ComponentAddName"))
			{
				NameComponent* name = ComponentManager::addEntityComponent<NameComponent>(_selected);
				if(name != nullptr)
					name->name[0] = '\0';
			}

			if(ImGui::Selectable("Transform##ComponentAddTransform"))
			{
				TransformComponent defTrans;
				TransformComponent* transform = ComponentManager::addEntityComponent<TransformComponent>(_selected);
				*transform = defTrans;

			}

			if(ImGui::Selectable("Mesh##ComponentAddMesh"))
			{
				MeshComponent* mesh = ComponentManager::addEntityComponent<MeshComponent>(_selected);
				mesh->sid = StringId(fs::absolute("../resources/meshes/plane.obj").string());
			}

			if(ImGui::Selectable("Script##ComponentAddMesh"))
			{
				ScriptComponent* script = ComponentManager::addEntityComponent<ScriptComponent>(_selected);
				script->sid = StringId("scriptCPU");
			}

			ImGui::EndPopup();
		}

		NameComponent* name = ComponentManager::getEntityComponent<NameComponent>(_selected);
		if(name != nullptr)
			if(ImGui::CollapsingHeader("Name##ComponentsNameHeader", ImGuiTreeNodeFlags_None))
            	ImGui::InputText("Name", name->name, sizeof(NameComponent::name));

		TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(_selected);
		if(transform != nullptr)
			if(ImGui::CollapsingHeader("Transform##ComponentsTransformHeader", ImGuiTreeNodeFlags_None))
			{
				bool posChanged = false, oriChanged = false, scaleChanged = false;
				vec3 pos, ori, scale;
				pos = transform->position;
				ori = degrees(transform->orientation.toEuler());
				scale = transform->scale;

				ImGui::Text("Position");
				posChanged |= ImGui::DragFloat("X##SceneTransformPosX", &pos.x, 0.005f, 1000.0f, 1000.0f, "%.6f", ImGuiSliderFlags_None);
				posChanged |= ImGui::DragFloat("Y##SceneTransformPosY", &pos.y, 0.005f, 1000.0f, 1000.0f, "%.6f", ImGuiSliderFlags_None);
				posChanged |= ImGui::DragFloat("Z##SceneTransformPosZ", &pos.z, 0.005f, 1000.0f, 1000.0f, "%.6f", ImGuiSliderFlags_None);

				ImGui::Text("Orientation");
				oriChanged |= ImGui::DragFloat("X##SceneTransformOriX", &ori.x, 0.5f, -360.0f, 360.0f, "%.2f", ImGuiSliderFlags_None);
				oriChanged |= ImGui::DragFloat("Y##SceneTransformOriY", &ori.y, 0.5f, -360.0f, 360.0f, "%.2f", ImGuiSliderFlags_None);
				oriChanged |= ImGui::DragFloat("Z##SceneTransformOriZ", &ori.z, 0.5f, -360.0f, 360.0f, "%.2f", ImGuiSliderFlags_None);

				ImGui::Text("Scale");
				scaleChanged |= ImGui::DragFloat("X##SceneTransformScaleX", &scale.x, 0.005f, 0.0f, +FLT_MAX, "%.2f", ImGuiSliderFlags_None);
				scaleChanged |= ImGui::DragFloat("Y##SceneTransformScaleY", &scale.y, 0.005f, 0.0f, +FLT_MAX, "%.2f", ImGuiSliderFlags_None);
				scaleChanged |= ImGui::DragFloat("Z##SceneTransformScaleZ", &scale.z, 0.005f, 0.0f, +FLT_MAX, "%.2f", ImGuiSliderFlags_None);

				if(posChanged)
					transform->position = pos;
				if(oriChanged)
					transform->orientation = quat(radians(ori));
				if(scaleChanged)
					transform->scale = scale;
			}

		MeshComponent* mesh = ComponentManager::getEntityComponent<MeshComponent>(_selected);
		if(mesh != nullptr)
			if(ImGui::CollapsingHeader("Mesh##ComponentsMeshHeader", ImGuiTreeNodeFlags_None))
			{
				uint32_t comboValue;
				if(mesh->sid == StringId(fs::absolute("../resources/meshes/plane.obj").string()))
					comboValue = 0;
				else if(mesh->sid == StringId(fs::absolute("../resources/meshes/cube.obj").string()))
					comboValue = 1;
				else
					comboValue = 2;

				const char* names[] = { "Plane", "Cube", "Sphere" };
				const char* paths[] = { "../resources/meshes/plane.obj", "../resources/meshes/cube.obj", "../resources/meshes/sphere.obj"};
				const char* comboPreviewValue = names[comboValue];
				if(ImGui::BeginCombo("Mesh", comboPreviewValue))
				{
					for(uint32_t i = 0; i < sizeof(names)/sizeof(const char*); i++)
					{
						if(ImGui::Selectable(names[i], comboValue == i))
							mesh->sid = StringId(fs::absolute(paths[i]).string());
						if(comboValue == i)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
			}

		ScriptComponent* script = ComponentManager::getEntityComponent<ScriptComponent>(_selected);
		if(script != nullptr)
			if(ImGui::CollapsingHeader("Script##ComponentsScriptHeader", ImGuiTreeNodeFlags_None))
			{
				uint32_t comboValue;
				if(script->sid == StringId("scriptCPU"))
					comboValue = 0;
				else
					comboValue = 1;

				const char* names[] = { "scriptCPU", "scriptGPU" };
				const char* comboPreviewValue = names[comboValue];
				if(ImGui::BeginCombo("Script", comboPreviewValue))
				{
					for(uint32_t i = 0; i < sizeof(names)/sizeof(const char*); i++)
					{
						if(ImGui::Selectable(names[i], comboValue == i))
							script->sid = StringId(names[i]);
						if(comboValue == i)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
			}
	}

	bool ScenePanel::getSelected(EntityId& eid)
	{
		eid = _selected;
		return _someSelected;
	}
}
