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

#include <atta/resourceSystem/resourceManager.h>
#include <atta/resourceSystem/resources/mesh.h>

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
	}

	FastRenderer::~FastRenderer()
	{

	}

	void FastRenderer::render()
	{
		_geometryPipeline->begin();
		{
			Mesh* mesh = ResourceManager::get<Mesh>("../resources/meshes/cube.obj");
			GraphicsManager::getRendererAPI()->renderMesh(mesh->getId());
		}
		_geometryPipeline->end();
		GraphicsManager::getRendererAPI()->framebufferToScreen(_geometryPipeline->getRenderPass()->getFramebuffer());
	}
}
