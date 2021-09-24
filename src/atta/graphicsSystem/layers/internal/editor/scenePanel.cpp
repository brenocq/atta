//--------------------------------------------------
// Atta Graphics System
// scenePanel.cpp
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/layers/internal/editor/scenePanel.h>
#include <atta/componentSystem/components/components.h>
#include <atta/scriptSystem/scriptManager.h>
#include <atta/resourceSystem/resourceManager.h>
#include <atta/resourceSystem/resources/mesh.h>
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
				TransformComponent* transform = ComponentManager::addEntityComponent<TransformComponent>(_selected);
				*transform = TransformComponent{};
			}

			if(ImGui::Selectable("Mesh##ComponentAddMesh"))
			{
				MeshComponent* mesh = ComponentManager::addEntityComponent<MeshComponent>(_selected);
				mesh->sid = StringId(fs::path("meshes/cube.obj").string());
			}

			if(ImGui::Selectable("Material##ComponentAddMaterial"))
			{
				MaterialComponent* material = ComponentManager::addEntityComponent<MaterialComponent>(_selected);
				*material = MaterialComponent{};
			}

			ImGui::Separator();

			if(ImGui::Selectable("Point Light##ComponentAddPointLight"))
			{
				PointLightComponent* pl = ComponentManager::addEntityComponent<PointLightComponent>(_selected);
				*pl = PointLightComponent{};
			}

			if(ImGui::Selectable("Directional Light##ComponentAddDirectionalLight"))
			{
				DirectionalLightComponent* dl = ComponentManager::addEntityComponent<DirectionalLightComponent>(_selected);
				*dl = DirectionalLightComponent{};
			}

			ImGui::Separator();

			if(ImGui::Selectable("Script##ComponentAddScript"))
			{
				ScriptComponent* script = ComponentManager::addEntityComponent<ScriptComponent>(_selected);
				std::vector<StringId> scriptSids = ScriptManager::getScriptSids();
				if(scriptSids.size())
					script->sid = scriptSids[0];
			}

			if(ImGui::Selectable("Prototype##ComponentAddPrototype"))
			{
				PrototypeComponent* prototype = ComponentManager::addEntityComponent<PrototypeComponent>(_selected);
				prototype->maxClones = 1;
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
				std::vector<StringId> meshSids = ResourceManager::getResources<Mesh>();

				uint32_t comboValue;

				int i = 0;
				for(auto sid : meshSids)
				{
					if(mesh->sid == sid)
						comboValue = i;
					i++;	
				}

				const char* comboPreviewValue = fs::path(meshSids[comboValue].getString()).stem().string().c_str();
				if(ImGui::BeginCombo("Mesh", comboPreviewValue))
				{
					for(size_t i = 0; i < meshSids.size(); i++)
					{
						if(ImGui::Selectable(fs::path(meshSids[i].getString()).stem().string().c_str(), comboValue == i))
							mesh->sid = meshSids[i];
						if(comboValue == i)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}

		MaterialComponent* material = ComponentManager::getEntityComponent<MaterialComponent>(_selected);
		if(material != nullptr)
			if(ImGui::CollapsingHeader("Material##ComponentsMaterialHeader", ImGuiTreeNodeFlags_None))
			{
				static bool albedoIsTexture = false;
				static bool metallicIsTexture = false;
				static bool roughnessIsTexture = false;
				static bool aoIsTexture = false;
				static bool normalIsTexture = false;

				//albedoIsTexture = material->albedo.x == -1.0f;
				//metallicIsTexture = material->metallic == -1.0f;
				//roughnessIsTexture = material->roughness == -1.0f;
				//aoIsTexture = material->ao == -1.0f;

				albedoIsTexture = material->albedoTexture.getId() != SID("Empty texture");
				metallicIsTexture = material->metallicTexture.getId() != SID("Empty texture");
				roughnessIsTexture = material->roughnessTexture.getId() != SID("Empty texture");
				aoIsTexture = material->aoTexture.getId() != SID("Empty texture");

				float min = 0.0f;
				float max = 1.0f;
				{
					ImGui::Text("Albedo");
					ImGui::SameLine();
					ImGui::Checkbox("Is texture##isTexAlbedo", &albedoIsTexture);
					if(!albedoIsTexture)
					{
						if(material->albedoTexture.getId() != SID("Empty texture") || material->albedo.x == -1)
						{
							material->albedoTexture = StringId("Empty texture");
							material->albedo.x = 1.0f;
							material->albedo.y = 0.0f;
							material->albedo.z = 1.0f;
						}

						ImGui::SliderScalar("R##SceneMaterialR", ImGuiDataType_Float, &material->albedo.x, &min, &max, "%.6f");
						ImGui::SliderScalar("G##SceneMaterialG", ImGuiDataType_Float, &material->albedo.y, &min, &max, "%.6f");
						ImGui::SliderScalar("B##SceneMaterialB", ImGuiDataType_Float, &material->albedo.z, &min, &max, "%.6f");
					}
					else
						textureCombo("albedoTextureCombo", material->albedoTexture);
				}
				{
					ImGui::Text("Metallic");
					ImGui::SameLine();
					ImGui::Checkbox("Is texture##isTexMetallic", &metallicIsTexture);
					if(!metallicIsTexture)
					{
						if(material->metallicTexture.getId() != SID("Empty texture") || material->metallic == -1)
						{
							material->metallicTexture = StringId("Empty texture");
							material->metallic = 0.0f;
						}

						ImGui::SliderScalar("##SceneMaterialMetallic", ImGuiDataType_Float, &material->metallic, &min, &max, "%.6f");
					}
					else
						textureCombo("metallicTextureCombo", material->metallicTexture);
				}
				{
					ImGui::Text("Roughness");
					ImGui::SameLine();
					ImGui::Checkbox("Is texture##isTexRoughness", &roughnessIsTexture);
					if(!roughnessIsTexture)
					{
						if(material->roughnessTexture.getId() != SID("Empty texture") || material->roughness == -1)
						{
							material->roughnessTexture = StringId("Empty texture");
							material->roughness = 0.0f;
						}

						ImGui::SliderScalar("##SceneMaterialRoughness", ImGuiDataType_Float, &material->roughness, &min, &max, "%.6f");
					}
					else
						textureCombo("roughnessTextureCombo", material->roughnessTexture);
				}
				{
					ImGui::Text("AO");
					ImGui::SameLine();
					ImGui::Checkbox("Is texture##isTexAO", &aoIsTexture);
					if(!aoIsTexture)
					{
						if(material->aoTexture.getId() != SID("Empty texture") || material->ao == -1)
						{
							material->aoTexture = StringId("Empty texture");
							material->ao = 1.0f;
						}

						ImGui::SliderScalar("##SceneMaterialAO", ImGuiDataType_Float, &material->ao, &min, &max, "%.6f");
					}
					else
						textureCombo("aoTextureCombo", material->aoTexture);
				}
				{
					ImGui::Text("Normal");
					ImGui::SameLine();
					ImGui::Checkbox("Is texture##isTexNormal", &normalIsTexture);
				}
			}

		PointLightComponent* pointLight = ComponentManager::getEntityComponent<PointLightComponent>(_selected);
		if(pointLight != nullptr)
			if(ImGui::CollapsingHeader("PointLight##ComponentsMaterialHeader", ImGuiTreeNodeFlags_None))
			{
				float min = 0.0f;
				float max = 10.0f;
				ImGui::Text("Intensity");
				ImGui::SliderScalar("R##ScenePointLightR", ImGuiDataType_Float, &pointLight->intensity.x, &min, &max, "%.6f");
				ImGui::SliderScalar("G##ScenePointLightG", ImGuiDataType_Float, &pointLight->intensity.y, &min, &max, "%.6f");
				ImGui::SliderScalar("B##ScenePointLightB", ImGuiDataType_Float, &pointLight->intensity.z, &min, &max, "%.6f");
			}

		DirectionalLightComponent* directionalLight = ComponentManager::getEntityComponent<DirectionalLightComponent>(_selected);
		if(directionalLight != nullptr)
			if(ImGui::CollapsingHeader("DirectionalLight##ComponentsMaterialHeader", ImGuiTreeNodeFlags_None))
			{
				float min = 0.0f;
				float max = 1.0f;
				ImGui::Text("Intensity");
				ImGui::SliderScalar("R##SceneDirectionalLightR", ImGuiDataType_Float, &directionalLight->intensity.x, &min, &max, "%.6f");
				ImGui::SliderScalar("G##SceneDirectionalLightG", ImGuiDataType_Float, &directionalLight->intensity.y, &min, &max, "%.6f");
				ImGui::SliderScalar("B##SceneDirectionalLightB", ImGuiDataType_Float, &directionalLight->intensity.z, &min, &max, "%.6f");
			}

		ScriptComponent* script = ComponentManager::getEntityComponent<ScriptComponent>(_selected);
		if(script != nullptr)
			if(ImGui::CollapsingHeader("Script##ComponentsScriptHeader", ImGuiTreeNodeFlags_None))
			{
				std::vector<StringId> scriptSids = ScriptManager::getScriptSids();
				uint32_t comboValue = 0;
				for(size_t i = 0; i<scriptSids.size(); i++)
					if(script->sid == scriptSids[i])
						comboValue = i;

				const char* comboPreviewValue = scriptSids[comboValue].getString().c_str();
				if(ImGui::BeginCombo("Script", comboPreviewValue ))
				{
					for(size_t i = 0; i < scriptSids.size(); i++)
					{
						if(ImGui::Selectable(scriptSids[i].getString().c_str(), comboValue == i))
							script->sid = scriptSids[i];
						if(comboValue == i)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
			}

		PrototypeComponent* prototype = ComponentManager::getEntityComponent<PrototypeComponent>(_selected);
		if(prototype != nullptr)
			if(ImGui::CollapsingHeader("Prototype##ComponentsPrototypeHeader", ImGuiTreeNodeFlags_None))
			{
				int value = prototype->maxClones;
				ImGui::DragInt("MaxClones##ScenePrototypeMaxClones", &value, 1, 0, 1000000, "%d", ImGuiSliderFlags_AlwaysClamp);
				prototype->maxClones = (uint64_t)value;
			}
	}

	bool ScenePanel::getSelected(EntityId& eid)
	{
		eid = _selected;
		return _someSelected;
	}

	void ScenePanel::textureCombo(std::string comboId, StringId& sid)
	{
		std::vector<StringId> textures = ResourceManager::getResources<Texture>();

		if(sid == "Empty texture")
		{
			DASSERT(textures.size()>0, "At least one texture should be loaded to the memory");
			sid = textures[0];
		}


		if(ImGui::BeginCombo(("Texture##"+comboId).c_str(), fs::path(sid.getString()).filename().string().c_str()))
		{
			for(size_t i = 0; i < textures.size(); i++)
			{
				if(ImGui::Selectable(fs::path(textures[i].getString()).filename().string().c_str(), sid == textures[i]))
					sid = textures[i];
				if(textures[i] == sid)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}
}
