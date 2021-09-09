//--------------------------------------------------
// Atta Graphics System
// openGLRenderer.h
// Date: 2021-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_RENDERER_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_RENDERER_H
#include <atta/graphicsSystem/rendererAPI.h>
#include <atta/graphicsSystem/window.h>

struct GLFWwindow;
namespace atta
{
	class OpenGLRenderer final : public RendererAPI
	{
	public:
		OpenGLRenderer(std::shared_ptr<Window> window);
		~OpenGLRenderer();

		void beginFrame() override;
		void endFrame() override;

		void beginRenderPass() override;
		void endRenderPass() override;

		void renderMesh() override;
	
	private:
		std::shared_ptr<Window> _window;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_APIS_OPENGL_OPENGL_RENDERER_H
