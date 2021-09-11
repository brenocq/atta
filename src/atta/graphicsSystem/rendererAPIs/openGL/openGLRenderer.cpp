//--------------------------------------------------
// Atta Graphics System
// rendererOpenGL.h
// Date: 2021-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLRenderer.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/base.h>
#include <GLFW/glfw3.h>

namespace atta
{
		OpenGLRenderer::OpenGLRenderer(std::shared_ptr<Window> window):
			RendererAPI(RendererAPI::OPENGL), _window(window)
		{
			// Initialize GLAD
			int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
			ASSERT(status, "Failed to initialize Glad!");

			// Print info
			LOG_INFO("OpenGLRenderer", "GPU Info:");
			LOG_INFO("OpenGLRenderer", "  - Vendor: $0", glGetString(GL_VENDOR));
			LOG_INFO("OpenGLRenderer", "  - Renderer: $0", glGetString(GL_RENDERER));
			LOG_INFO("OpenGLRenderer", "  - Version: $0", glGetString(GL_VERSION));

#ifdef ATTA_DEBUG_BUILD
			// Enable Debug
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, 
						GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
					{
						switch (severity)
						{
						case GL_DEBUG_SEVERITY_HIGH:
							LOG_ERROR("OpenGL Debug", "$0", message);
							ASSERT(false, "OpenGL error");
							break;
						case GL_DEBUG_SEVERITY_MEDIUM:
							LOG_WARN("OpenGL Debug", "$0", message);
							break;
						case GL_DEBUG_SEVERITY_LOW:
							LOG_INFO("OpenGL Debug", "$0", message);
							break;
						case GL_DEBUG_SEVERITY_NOTIFICATION:
							//LOG_VERBOSE("OpenGL Debug", "$0", message);
							break;
						}
					}, nullptr);
#endif// ATTA_DEBUG_BUILD

			// Check OpenGL version
			int versionMajor;
			int versionMinor;
			glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
			glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
			ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Atta requires OpenGL >= 4.5");

			glEnable(GL_DEPTH_TEST);
		}

		OpenGLRenderer::~OpenGLRenderer()
		{

		}

		void OpenGLRenderer::beginFrame()
		{
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void OpenGLRenderer::endFrame()
		{

		}

		void OpenGLRenderer::beginRenderPass()
		{

		}

		void OpenGLRenderer::endRenderPass()
		{

		}

		void OpenGLRenderer::renderMesh()
		{

		}
}
