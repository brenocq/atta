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
			shader->setVec3("lightColor", {0.8f, 0.4f, 0.3f});

			for(auto entity : entities)
			{
				MeshComponent* mesh = ComponentManager::getEntityComponent<MeshComponent>(entity);
				TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);

				if(mesh != nullptr && transform != nullptr)
				{
					shader->setMat4("transform", transpose(posOriScale(transform->position, transform->orientation, transform->scale)));
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
