//--------------------------------------------------
// Atta Graphics System
// fastRenderer.cpp
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/renderers/fastRenderer.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/graphicsSystem/framebuffer.h>
#include <atta/graphicsSystem/renderPass.h>

#include <atta/graphicsSystem/rendererAPIs/openGL/openGLShaderGroup.h>

#include <atta/resourceSystem/resourceManager.h>
#include <atta/resourceSystem/resources/mesh.h>

#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/components/meshComponent.h>
#include <atta/componentSystem/components/transformComponent.h>

namespace atta
{
	FastRenderer::FastRenderer()
	{
		// Framebuffer
		Framebuffer::CreateInfo framebufferInfo {};
		framebufferInfo.attachments.push_back({Image::Format::RGB});
		framebufferInfo.width = 500;
		framebufferInfo.height = 500;
		framebufferInfo.debugName = StringId("Simulation Layer Framebuffer");
		std::shared_ptr<Framebuffer> framebuffer = GraphicsManager::create<Framebuffer>(framebufferInfo);

		// Shader Group
		ShaderGroup::CreateInfo shaderGroupInfo {};
		// TODO relative to resources directory
		shaderGroupInfo.shaderPaths = {"../resources/shaders/fastRenderer/shader.vert", "../resources/shaders/fastRenderer/shader.frag"};
		shaderGroupInfo.debugName = StringId("Simulation Layer Shader Group");
		std::shared_ptr<ShaderGroup> shaderGroup = GraphicsManager::create<ShaderGroup>(shaderGroupInfo);

		// Render Pass
		RenderPass::CreateInfo renderPassInfo {};
		renderPassInfo.framebuffer = framebuffer;
		renderPassInfo.debugName = StringId("Simulation Layer Render Pass");
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

		// XXX
		ResourceManager::get<Mesh>("../resources/meshes/plane.obj");
		ResourceManager::get<Mesh>("../resources/meshes/cube.obj");
		ResourceManager::get<Mesh>("../resources/meshes/sphere.obj");
	}

	FastRenderer::~FastRenderer()
	{

	}

	void FastRenderer::render()
	{
		std::vector<EntityId> entities = ComponentManager::getEntities();

		_geometryPipeline->begin();
		{
			for(auto entity : entities)
			{
				MeshComponent* mesh = ComponentManager::getEntityComponent<MeshComponent>(entity);
				TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);


				if(mesh != nullptr && transform != nullptr)
				{
					// XXX Move to shader class
					// Updating transform uniform
					std::shared_ptr<OpenGLShaderGroup> shader = std::static_pointer_cast<OpenGLShaderGroup>(_geometryPipeline->getShaderGroup());
					unsigned int transformLoc = glGetUniformLocation(shader->getId(), "transform");
					mat4 trans;
					trans.setPosOriScale(transform->position, transform->orientation, transform->scale);
					trans.transpose();
					glUniformMatrix4fv(transformLoc, 1, GL_FALSE, trans.data);

					// Draw mesh
					GraphicsManager::getRendererAPI()->renderMesh(mesh->sid);
				}
			}
		}
		_geometryPipeline->end();
		GraphicsManager::getRendererAPI()->framebufferToScreen(_geometryPipeline->getRenderPass()->getFramebuffer());
	}
}
