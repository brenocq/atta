//--------------------------------------------------
// Atta Graphics System
// pbrRenderer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERERS_PBR_RENDERER_H
#define ATTA_GRAPHICS_SYSTEM_RENDERERS_PBR_RENDERER_H
#include <atta/graphicsSystem/pipeline.h>
#include <atta/graphicsSystem/renderers/renderer.h>

namespace atta
{
	class PbrRenderer final : public Renderer
	{
	public:
		PbrRenderer();
		~PbrRenderer();

		void render(std::shared_ptr<Camera> camera) override;
		void resize(uint32_t width, uint32_t height) override;

		uint32_t getWidth() const override { return _geometryPipeline->getRenderPass()->getFramebuffer()->getWidth(); };
		uint32_t getHeight() const override { return _geometryPipeline->getRenderPass()->getFramebuffer()->getHeight(); };
		void* getImGuiTexture() const override { return _geometryPipeline->getImGuiTexture(); }

	private:
		void renderCube();
		void renderQuad();
		void generateCubemap();
		void convoluteCubemap();
		void prefilterCubemap();
		void brdfLUT();

		std::shared_ptr<Pipeline> _geometryPipeline;

		std::shared_ptr<ShaderGroup> _backgroundShader;
		unsigned int _envCubemap;
		unsigned int _irradianceMap;
		unsigned int _prefilterMap;
		unsigned int _brdfLUT;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERERS_PBR_RENDERER_H
