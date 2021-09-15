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
		//framebufferInfo.attachments.push_back({Image::Format::DEPTH32F});
		framebufferInfo.width = 500;
		framebufferInfo.height = 500;
		framebufferInfo.debugName = StringId("Simulation Layer Framebuffer");
		std::shared_ptr<Framebuffer> framebuffer = GraphicsManager::create<Framebuffer>(framebufferInfo);

		// Shader Group
		ShaderGroup::CreateInfo shaderGroupInfo {};
		// TODO relative to resources directory
		shaderGroupInfo.shaderPaths = {"shaders/fastRenderer/shader.vert", "shaders/fastRenderer/shader.frag"};
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
		ResourceManager::get<Mesh>("meshes/plane.obj");
		ResourceManager::get<Mesh>("meshes/cube.obj");
		ResourceManager::get<Mesh>("meshes/sphere.obj");
	}

	FastRenderer::~FastRenderer()
	{

	}

	void FastRenderer::render(std::shared_ptr<Camera> camera)
	{
		std::vector<EntityId> entities = ComponentManager::getEntities();
		_geometryPipeline->begin();
		{
			std::shared_ptr<OpenGLShaderGroup> shader = std::static_pointer_cast<OpenGLShaderGroup>(_geometryPipeline->getShaderGroup());

			unsigned int projLoc = glGetUniformLocation(shader->getId(), "projection");
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, transpose(camera->getProj()).data);
			//mat4 proj = mat4(1.0f);
			//glUniformMatrix4fv(projLoc, 1, GL_FALSE, proj.data);
			unsigned int viewLoc = glGetUniformLocation(shader->getId(), "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, transpose(camera->getView()).data);

			for(auto entity : entities)
			{
				MeshComponent* mesh = ComponentManager::getEntityComponent<MeshComponent>(entity);
				TransformComponent* transform = ComponentManager::getEntityComponent<TransformComponent>(entity);

				if(mesh != nullptr && transform != nullptr)
				{
					// XXX Move to shader class
					// Updating transform uniform

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
		//GraphicsManager::getRendererAPI()->framebufferToScreen(_geometryPipeline->getRenderPass()->getFramebuffer());
	}

	void FastRenderer::resize(uint32_t width, uint32_t height)
	{
		if(width != _width || height != _height)
		{
			_geometryPipeline->getRenderPass()->getFramebuffer()->resize(width, height);
			_width = width;
			_height = height;
		}
	}
}
