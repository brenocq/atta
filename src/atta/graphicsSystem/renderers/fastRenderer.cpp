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

namespace atta
{
	FastRenderer::FastRenderer()
	{
		// Framebuffer
		Framebuffer::CreateInfo framebufferInfo {};
		framebufferInfo.attachments.push_back({Image::Format::RGBA});
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
			{ "inColor", VertexBufferElement::Type::VEC3 },
		};
		pipelineInfo.renderPass = renderPass;
		_geometryPipeline = GraphicsManager::create<Pipeline>(pipelineInfo);
	}

	FastRenderer::~FastRenderer()
	{

	}
}
