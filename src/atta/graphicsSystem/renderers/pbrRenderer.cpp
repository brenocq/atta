//--------------------------------------------------
// Atta Graphics System
// pbrRenderer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/renderers/pbrRenderer.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/graphicsSystem/framebuffer.h>
#include <atta/graphicsSystem/renderPass.h>

#include <atta/resourceSystem/resourceManager.h>
#include <atta/resourceSystem/resources/mesh.h>

#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/meshComponent.h>
#include <atta/componentSystem/components/transformComponent.h>
#include <atta/componentSystem/components/materialComponent.h>
#include <atta/componentSystem/components/pointLightComponent.h>
#include <atta/componentSystem/components/directionalLightComponent.h>
#include <atta/componentSystem/factory.h>

namespace atta
{
	PbrRenderer::PbrRenderer()
	{
		//---------- Create geometry pipeline ----------//
		// Framebuffer
		Framebuffer::CreateInfo framebufferInfo {};
		framebufferInfo.attachments.push_back({Image::Format::RGB});
		framebufferInfo.attachments.push_back({Image::Format::DEPTH32F});
		framebufferInfo.width = 500;
		framebufferInfo.height = 500;
		framebufferInfo.debugName = StringId("PbrRenderer Framebuffer");
		std::shared_ptr<Framebuffer> framebuffer = GraphicsManager::create<Framebuffer>(framebufferInfo);

		// Shader Group
		ShaderGroup::CreateInfo shaderGroupInfo {};
		shaderGroupInfo.shaderPaths = {"shaders/pbrRenderer/shader.vert", "shaders/pbrRenderer/shader.frag"};
		shaderGroupInfo.debugName = StringId("PbrRenderer Shader Group");
		std::shared_ptr<ShaderGroup> shaderGroup = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

		// Render Pass
		RenderPass::CreateInfo renderPassInfo {};
		renderPassInfo.framebuffer = framebuffer;
		renderPassInfo.debugName = StringId("PbrRenderer Render Pass");
		std::shared_ptr<RenderPass> renderPass = GraphicsManager::create<RenderPass>(renderPassInfo);

		Pipeline::CreateInfo pipelineInfo {};
		// Vertex input layout
		pipelineInfo.shaderGroup = shaderGroup;
		pipelineInfo.layout = {
			{ "inPosition", VertexBufferElement::Type::VEC3 },
			{ "inNormal", VertexBufferElement::Type::VEC3 },
			{ "inTexCoord", VertexBufferElement::Type::VEC2 }
		};
		pipelineInfo.renderPass = renderPass;
		_geometryPipeline = GraphicsManager::create<Pipeline>(pipelineInfo);
	}

	PbrRenderer::~PbrRenderer()
	{

	}

	void PbrRenderer::render(std::shared_ptr<Camera> camera)
	{
		std::vector<EntityId> entities = ComponentManager::getEntities();
		_geometryPipeline->begin();
		{
			std::shared_ptr<ShaderGroup> shader = _geometryPipeline->getShaderGroup();
			shader->setMat4("projection", transpose(camera->getProj()));
			shader->setMat4("view", transpose(camera->getView()));
			shader->setVec3("camPos", camera->getPosition());

			//----- Lighting -----//
			int numPointLights = 0;
			for(auto entity : entities)
			{
				TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);
				PointLightComponent* pl = ComponentManager::getEntityComponent<PointLightComponent>(entity);
				DirectionalLightComponent* dl = ComponentManager::getEntityComponent<DirectionalLightComponent>(entity);

				if(transform && (pl || dl))
				{
					if(pl && numPointLights < 10)
					{
						int i = numPointLights++;
						shader->setVec3(("pointLights["+std::to_string(i)+"].position").c_str(), transform->position);
						shader->setVec3(("pointLights["+std::to_string(i)+"].intensity").c_str(), pl->intensity);
					}
					if(dl)
					{
						vec3 before = { 0.0f, -1.0f, 0.0f };
						//vec3 direction;
						//transform->orientation.transformVector(before, direction);
						shader->setVec3("directionalLight.direction", before);
						shader->setVec3("directionalLight.intensity", dl->intensity);
					}
					if(numPointLights++ == 10)
						LOG_WARN("PhongRenderer", "Maximum number of point lights reached, 10 lights");
				}
			}
			shader->setInt("numPointLights", numPointLights);

			for(auto entity : entities)
			{
				MeshComponent* mesh = ComponentManager::getEntityComponent<MeshComponent>(entity);
				TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);
				MaterialComponent* material = ComponentManager::getEntityComponent<MaterialComponent>(entity);

				if(mesh && transform)
				{
					mat4 model; 
					model.setPosOriScale(transform->position, transform->orientation, transform->scale);
					model.transpose();
					mat4 invModel = inverse(model);
					shader->setMat4("model", model);
					shader->setMat4("invModel", invModel);

					if(material)
					{
						if(material->albedoTexture.getId() != SID("Empty texture"))
						{
							shader->setTexture("material.albedoTexture", material->albedoTexture);
							shader->setVec3("material.albedo", {-1, -1, -1});
						}
						else
							shader->setVec3("material.albedo", material->albedo);

						if(material->metallicTexture.getId() != SID("Empty texture"))
						{
							shader->setTexture("material.metallicTexture", material->metallicTexture);
							shader->setFloat("material.metallic", -1);
						}
						else
							shader->setFloat("material.metallic", material->metallic);

						if(material->roughnessTexture.getId() != SID("Empty texture"))
						{
							shader->setTexture("material.roughnessTexture", material->roughnessTexture);
							shader->setFloat("material.roughness", -1);
						}
						else
							shader->setFloat("material.roughness", material->roughness);

						if(material->aoTexture.getId() != SID("Empty texture"))
						{
							shader->setTexture("material.aoTexture", material->aoTexture);
							shader->setFloat("material.ao", -1);
						}
						else
							shader->setFloat("material.ao", material->ao);

						if(material->normalTexture.getId() != SID("Empty texture"))
						{
							shader->setTexture("material.normalTexture", material->normalTexture);
							shader->setInt("material.hasNormalTexture", 1);
						}
						else
							shader->setInt("material.hasNormalTexture", 0);
					}
					else
					{
						MaterialComponent material {};
						shader->setVec3("material.albedo", material.albedo);
						shader->setFloat("material.metallic", material.metallic);
						shader->setFloat("material.roughness", material.roughness);
						shader->setFloat("material.ao", material.ao);
						shader->setFloat("material.hasNormalTexture", 0);
					}

					GraphicsManager::getRendererAPI()->renderMesh(mesh->sid);
				}
			}
		}
		_geometryPipeline->end();
	}

	void PbrRenderer::resize(uint32_t width, uint32_t height)
	{
		if(width != _width || height != _height)
		{
			_geometryPipeline->getRenderPass()->getFramebuffer()->resize(width, height);
			_width = width;
			_height = height;
		}
	}
}
