//--------------------------------------------------
// Atta File System
// projectSerializer.cpp
// Date: 2021-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/fileSystem/project/projectSerializer.h>
#include <atta/cmakeConfig.h>

#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/components.h>

#include <atta/resourceSystem/resourceManager.h>
#include <atta/resourceSystem/resources/mesh.h>
#include <atta/resourceSystem/resources/texture.h>

#define WRITE_BIN(s,x)
#define WRITE_VEC_BIN(s,vec,size) s.write(reinterpret_cast<const char*>(vec), size);

namespace atta
{
	ProjectSerializer::ProjectSerializer(std::shared_ptr<Project> project):
		_project(project)
	{
	}

	ProjectSerializer::~ProjectSerializer()
	{
	}

	void ProjectSerializer::serialize()
	{
		fs::path attaTemp = _project->getFile();
		attaTemp.replace_extension(".atta.temp");

		std::ofstream os(attaTemp, std::ofstream::trunc | std::ofstream::binary);

		serializeHeader(os);
		serializeComponentSystem(os);

		os.close();

		// Override atta file with temp file
		fs::rename(attaTemp, _project->getFile());
	}

	void ProjectSerializer::deserialize()
	{
		fs::path attaFile = _project->getFile();

		std::ifstream is(attaFile, std::ifstream::in | std::ifstream::binary);

		Header header = deserializeHeader(is);
		//LOG_WARN("ProjectSerializer", "[*y]Header:[]\n\tversion:$0.$1.$2.$3\n\tproj:$4\n\tcounter:$5", 
		//		header.version[0], header.version[1], header.version[2], header.version[3], 
		//		header.projectName, header.saveCounter);

		if(is)
		{
			std::string marker;
			read(is, marker);
			if(marker == "comp")
				deserializeComponentSystem(is);
		}

		is.close();
	}

	void ProjectSerializer::serializeHeader(std::ofstream& os)
	{
		// Atta info
		write(os, "atta");
		uint16_t version[] = { ATTA_VERSION_MAJOR, ATTA_VERSION_MINOR, ATTA_VERSION_PATCH, ATTA_VERSION_TWEAK };
		write(os, version);// Version

		// Project info
		write(os, "proj");
		write(os, _project->getName());// Project name
		uint32_t saveCounter = 0;
		write(os, saveCounter);// Save counter (number of times that was saved)
		write(os, "hend");
	}

	ProjectSerializer::Header ProjectSerializer::deserializeHeader(std::ifstream& is)
	{
		std::string marker;
		Header header;

		while(true)
		{
			read(is, marker);
			if(marker == "hend")
			{
				return header;
			}
			else if(marker == "atta")
			{
				read(is, header.version);
			}
			else if(marker == "proj")
			{
				read(is, header.projectName);
				read(is, header.saveCounter);
			}
			else
			{
				LOG_WARN("ProjectSerializer", "Unknown marker found at the header [w]$0[]", marker);
				return header;
			}
		}
		return header;
	}

	void ProjectSerializer::serializeComponentSystem(std::ofstream& os)
	{
		// Component system marker
		write(os, "comp");

		// Serialize entity ids
		std::vector<EntityId> entities = ComponentManager::getEntities();
		write(os, "id");// Entity id marker
		write<uint32_t>(os, entities.size());
		for(EntityId entity : entities)
			write(os, entity);

		// Serialize components
		std::vector<std::string> componentNames = ComponentManager::getComponentNames();
		write<uint32_t>(os, componentNames.size());// Number of components

		// Write each component (O(n*m), TODO faster)
		// TODO support for custom components
		for(auto component : componentNames)
		{
			write(os, component);
			if(component == "Script")
			{
				std::vector<std::pair<EntityId,ScriptComponent*>> scripts;
				for(auto entity : entities)
				{
					ScriptComponent* script = ComponentManager::getEntityComponent<ScriptComponent>(entity);
					if(script != nullptr)
						scripts.push_back(std::make_pair(entity, script));
				}

				write<uint32_t>(os, scripts.size());
				for(auto script : scripts)
				{
					write(os, script.first);
					write(os, script.second->sid);
				}
			}
			else if(component == "Mesh")
			{
				std::vector<std::pair<EntityId,MeshComponent*>> meshes;
				for(auto entity : entities)
				{
					MeshComponent* mesh = ComponentManager::getEntityComponent<MeshComponent>(entity);
					if(mesh != nullptr)
						meshes.push_back(std::make_pair(entity, mesh));
				}
				write<uint32_t>(os, meshes.size());
				for(auto mesh : meshes)
				{
					write(os, mesh.first);
					write(os, mesh.second->sid.getString());
				}
			}
			else if(component == "Name")
			{
				std::vector<std::pair<EntityId,NameComponent*>> names;
				for(auto entity : entities)
				{
					NameComponent* name = ComponentManager::getEntityComponent<NameComponent>(entity);
					if(name != nullptr)
						names.push_back(std::make_pair(entity, name));
				}

				write<uint32_t>(os, names.size());
				for(auto name : names)
				{
					write(os, name.first);
					write(os, name.second->name);
				}
			}
			else if(component == "Transform")
			{
				std::vector<std::pair<EntityId,TransformComponent*>> transforms;
				for(auto entity : entities)
				{
					TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);
					if(transform != nullptr)
						transforms.push_back(std::make_pair(entity, transform));
				}

				write<uint32_t>(os, transforms.size());
				for(auto transform : transforms)
				{
					write(os, transform.first);
					write(os, transform.second->position);
					write(os, transform.second->orientation);
					write(os, transform.second->scale);
				}
			}
			else if(component == "Material")
			{
				std::vector<std::pair<EntityId,MaterialComponent*>> materials;
				for(auto entity : entities)
				{
					MaterialComponent* material = ComponentManager::getEntityComponent<MaterialComponent>(entity);
					if(material != nullptr)
						materials.push_back(std::make_pair(entity, material));
				}

				write<uint32_t>(os, materials.size());
				for(auto material : materials)
				{
					write(os, material.first);
					write(os, material.second->albedo);
					write(os, material.second->metallic);
					write(os, material.second->roughness);
					write(os, material.second->ao);
					write(os, material.second->albedoTexture.getString());
					write(os, material.second->metallicTexture.getString());
					write(os, material.second->roughnessTexture.getString());
					write(os, material.second->aoTexture.getString());
					write(os, material.second->normalTexture.getString());
				}
			}
			else if(component == "Point Light")
			{
				std::vector<std::pair<EntityId,PointLightComponent*>> pointLights;
				for(auto entity : entities)
				{
					PointLightComponent* pointLight = ComponentManager::getEntityComponent<PointLightComponent>(entity);
					if(pointLight != nullptr)
						pointLights.push_back(std::make_pair(entity, pointLight));
				}

				write<uint32_t>(os, pointLights.size());
				for(auto pointLight : pointLights)
				{
					write(os, pointLight.first);
					write(os, pointLight.second->intensity);
				}
			}
			else if(component == "Directional Light")
			{
				std::vector<std::pair<EntityId,DirectionalLightComponent*>> directionalLights;
				for(auto entity : entities)
				{
					DirectionalLightComponent* directionalLight = ComponentManager::getEntityComponent<DirectionalLightComponent>(entity);
					if(directionalLight != nullptr)
						directionalLights.push_back(std::make_pair(entity, directionalLight));
				}

				write<uint32_t>(os, directionalLights.size());
				for(auto directionalLight : directionalLights)
				{
					write(os, directionalLight.first);
					write(os, directionalLight.second->intensity);
				}
			}
			else if(component == "Prototype")
			{
				std::vector<std::pair<EntityId,PrototypeComponent*>> prototypes;
				for(auto entity : entities)
				{
					PrototypeComponent* prototype = ComponentManager::getEntityComponent<PrototypeComponent>(entity);
					if(prototype != nullptr)
						prototypes.push_back(std::make_pair(entity, prototype));
				}

				write<uint32_t>(os, prototypes.size());
				for(auto prototype : prototypes)
				{
					write(os, prototype.first);
					write(os, prototype.second->maxClones);
				}
			}
			else if(component == "Relationship")
			{
				std::vector<std::pair<EntityId,RelationshipComponent*>> relationships;
				for(auto entity : entities)
				{
					RelationshipComponent* relationship = ComponentManager::getEntityComponent<RelationshipComponent>(entity);
					if(relationship != nullptr)
						relationships.push_back(std::make_pair(entity, relationship));
				}

				write<uint32_t>(os, relationships.size());
				for(auto relationship : relationships)
				{
					write(os, relationship.first);
					write(os, relationship.second->parent);
					write<uint32_t>(os, relationship.second->children.size());
					for(auto child : relationship.second->children)
						write(os, child);
				}
			}
		}
	}

	void ProjectSerializer::deserializeComponentSystem(std::ifstream& is)
	{
		std::string marker;
		uint32_t numComponents = 0;

		// Read entity ids
		{
			read(is, marker);
			if(marker != "id")
			{
				LOG_WARN("ProjectSerializer", "First component marker must be [w]id[], but it is [w]$0[]. Could not load atta file", marker);
				return;
			}

			uint32_t numIds;
			read(is, numIds);
			LOG_VERBOSE("ProjectSerializer","Num ids: $0", numIds);
			for(uint32_t i = 0; i < numIds; i++)
			{
				EntityId id;
				read(is, id);
				LOG_VERBOSE("ProjectSerializer","id: $0", id);

				EntityId res = ComponentManager::createEntity();
				if(res != id)
					LOG_VERBOSE("ProjectSerializer","Created and serialized entities does not match! $0 and $1", id, res);
			}
		}

		// Read number of components
		read(is, numComponents);

		for(uint32_t i = 0; i < numComponents; i++)
		{
			read(is, marker);
			if(marker == "Script")
			{
				uint32_t numScripts;
				read<uint32_t>(is, numScripts);
				LOG_VERBOSE("ProjectSerializer","Num scripts: $0", numScripts);
				for(uint32_t i = 0; i < numScripts; i++)
				{
					EntityId eid;
					read(is, eid);
					StringId sid;
					read(is, sid);

					ScriptComponent* script = ComponentManager::addEntityComponent<ScriptComponent>(eid);
					script->sid = sid;
					LOG_VERBOSE("ProjectSerializer","entity($0) -> $1", eid, sid);
				}
			}
			else if(marker == "Mesh")
			{
				uint32_t numMeshes;
				read<uint32_t>(is, numMeshes);
				LOG_VERBOSE("ProjectSerializer","Num meshes: $0", numMeshes);
				for(uint32_t i = 0; i < numMeshes; i++)
				{
					EntityId eid;
					read(is, eid);
					std::string meshFile;
					read(is, meshFile);
					StringId sid = StringId(meshFile);
					//std::string meshFile = sid.getString();

					// Add entity mesh component
					MeshComponent* mesh = ComponentManager::addEntityComponent<MeshComponent>(eid);
					mesh->sid = sid;
					// Load mesh if not already loaded

					LOG_VERBOSE("ProjectSerializer","entity($0) -> $1", eid, meshFile);
					ResourceManager::get<Mesh>(meshFile);
				}
			}
			else if(marker == "Name")
			{
				uint32_t numNames;
				read<uint32_t>(is, numNames);
				LOG_VERBOSE("ProjectSerializer","Num names: $0", numNames);
				for(uint32_t i = 0; i < numNames; i++)
				{
					EntityId eid;
					read(is, eid);
					NameComponent temp;
					read(is, temp.name);

					NameComponent* name = ComponentManager::addEntityComponent<NameComponent>(eid);
					*name = temp;
					LOG_VERBOSE("ProjectSerializer","entity($0) -> $1", eid, name->name);
				}
			}
			else if(marker == "Transform")
			{
				uint32_t numTransforms;
				read<uint32_t>(is, numTransforms);
				LOG_VERBOSE("ProjectSerializer","Num transforms: $0", numTransforms);
				for(uint32_t i = 0; i < numTransforms; i++)
				{
					EntityId eid;
					read(is, eid);
					TransformComponent temp;
					read(is, temp.position);
					read(is, temp.orientation);
					read(is, temp.scale);

					TransformComponent* transform = ComponentManager::addEntityComponent<TransformComponent>(eid);
					*transform = temp;

					LOG_VERBOSE("ProjectSerializer","entity($0) -> p:$1 o:$2 s:$3", 
							eid, transform->position, transform->orientation, transform->scale);
				}
			}
			else if(marker == "Material")
			{
				uint32_t numMaterials;
				read<uint32_t>(is, numMaterials);
				LOG_VERBOSE("ProjectSerializer","Num materials: $0", numMaterials);
				for(uint32_t i = 0; i < numMaterials; i++)
				{
					EntityId eid;
					read(is, eid);
					MaterialComponent temp;
					read(is, temp.albedo);
					read(is, temp.metallic);
					read(is, temp.roughness);
					read(is, temp.ao);
					std::string tempStr;
					read(is, tempStr); temp.albedoTexture = StringId(tempStr);
					read(is, tempStr); temp.metallicTexture = StringId(tempStr);
					read(is, tempStr); temp.roughnessTexture = StringId(tempStr);
					read(is, tempStr); temp.aoTexture = StringId(tempStr);
					read(is, tempStr); temp.normalTexture = StringId(tempStr);

					if(temp.albedoTexture.getId() != SID("Empty texture"))
						ResourceManager::get<Texture>(temp.albedoTexture.getString());
					if(temp.metallicTexture.getId() != SID("Empty texture"))
						ResourceManager::get<Texture>(temp.metallicTexture.getString());
					if(temp.roughnessTexture.getId() != SID("Empty texture"))
						ResourceManager::get<Texture>(temp.roughnessTexture.getString());
					if(temp.aoTexture.getId() != SID("Empty texture"))
						ResourceManager::get<Texture>(temp.aoTexture.getString());
					if(temp.normalTexture.getId() != SID("Empty texture"))
						ResourceManager::get<Texture>(temp.normalTexture.getString());

					MaterialComponent* material = ComponentManager::addEntityComponent<MaterialComponent>(eid);
					*material = temp;
				}
			}
			else if(marker == "Point Light")
			{
				uint32_t numPointLights;
				read<uint32_t>(is, numPointLights);
				LOG_VERBOSE("ProjectSerializer","Num point lights: $0", numPointLights);
				for(uint32_t i = 0; i < numPointLights; i++)
				{
					EntityId eid;
					read(is, eid);
					PointLightComponent temp;
					read(is, temp.intensity);

					PointLightComponent* pointLight = ComponentManager::addEntityComponent<PointLightComponent>(eid);
					*pointLight = temp;
				}
			}
			else if(marker == "Directional Light")
			{
				uint32_t numDirectionalLights;
				read<uint32_t>(is, numDirectionalLights);
				LOG_VERBOSE("ProjectSerializer","Num directional lights: $0", numDirectionalLights);
				for(uint32_t i = 0; i < numDirectionalLights; i++)
				{
					EntityId eid;
					read(is, eid);
					DirectionalLightComponent temp;
					read(is, temp.intensity);

					DirectionalLightComponent* directionalLight = ComponentManager::addEntityComponent<DirectionalLightComponent>(eid);
					*directionalLight = temp;
				}
			}
			else if(marker == "Prototype")
			{
				uint32_t numPrototypes;
				read<uint32_t>(is, numPrototypes);
				LOG_VERBOSE("ProjectSerializer","Num prototypes: $0", numPrototypes);
				for(uint32_t i = 0; i < numPrototypes; i++)
				{
					EntityId eid;
					read(is, eid);
					PrototypeComponent temp {};
					read(is, temp.maxClones);

					// Add entity prototype component
					PrototypeComponent* prototype = ComponentManager::addEntityComponent<PrototypeComponent>(eid);
					*prototype = temp;
				}
			}
			else if(marker == "Relationship")
			{
				uint32_t numRelationships;
				read<uint32_t>(is, numRelationships);
				LOG_VERBOSE("ProjectSerializer","Num relationships: $0", numRelationships);
				for(uint32_t i = 0; i < numRelationships; i++)
				{
					EntityId eid;
					read(is, eid);
					RelationshipComponent temp {};
					read(is, temp.parent);
					uint32_t numChildren;
					read(is, numChildren);
					for(size_t i = 0; i < numChildren; i++)
					{
						EntityId child;
						read(is, child);
						temp.children.push_back(child);
					}

					// Add entity relationship component
					RelationshipComponent* relationship = ComponentManager::addEntityComponent<RelationshipComponent>(eid);
					*relationship = temp;
				}
			}
			else
			{
				LOG_WARN("ProjectSerializer", "Unknown marker found at the component section: [w]$0[]", marker);
			}
		}
	}
}

