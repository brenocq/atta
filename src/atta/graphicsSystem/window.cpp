//--------------------------------------------------
// Atta Window
// window.cpp
// Date: 2021-08-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/window.h>
#include <atta/eventSystem/events/windowCloseEvent.h>
#include <atta/eventSystem/events/windowResizeEvent.h>
#include <atta/eventSystem/events/windowFocusEvent.h>
#include <atta/eventSystem/events/windowMouseMoveEvent.h>
#include <atta/eventSystem/events/windowMouseScrollEvent.h>
#include <atta/eventSystem/events/windowMouseButtonEvent.h>
#include <atta/eventSystem/events/windowKeyboardButtonEvent.h>
// ImGui backends
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace atta
{
	unsigned Window::_glfwWindowCounter = 0;

	Window::Window(const CreateInfo& info):
		_title(info.title), _width(info.width), _height(info.height),
		_eventManager(info.eventManager)
	{
		//if(_glfwWindowCounter++ == 0) XXX
			glfwInit();

		_window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(_window, this);

		glfwSetWindowCloseCallback(_window, [](GLFWwindow* window)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);
			WindowCloseEvent e;
			w._eventManager->publish(e);
		});

		glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);
			w._width = width;
			w._height = height;

			WindowResizeEvent e((size_t)width, (size_t)height);
			w._eventManager->publish(e);
		});

		glfwSetCursorEnterCallback(_window, [](GLFWwindow* window, int entered)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);
			WindowFocusEvent e(entered != 0);
			w._eventManager->publish(e);
		});

		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);
			WindowMouseMoveEvent e((float)xPos, (float)yPos);
			w._eventManager->publish(e);
		});

		glfwSetScrollCallback(_window, [](GLFWwindow* window, double dx, double dy)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);
			WindowMouseScrollEvent e((float)dx, (float)dy);
			w._eventManager->publish(e);
		});

		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);

			if(action == GLFW_PRESS)
			{
				WindowMouseButtonEvent e(button, WindowMouseButtonEvent::Action::PRESS);
				w._eventManager->publish(e);
			}
			else if(action == GLFW_RELEASE)
			{
				WindowMouseButtonEvent e(button, WindowMouseButtonEvent::Action::RELEASE);
				w._eventManager->publish(e);
			}
		});

		glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window& w = *(Window*)glfwGetWindowUserPointer(window);

			WindowKeyboardButtonEvent::Action a;
			switch(action)
			{
				case GLFW_PRESS:
					a = WindowKeyboardButtonEvent::Action::PRESS;
					break;
				case GLFW_REPEAT:
					a = WindowKeyboardButtonEvent::Action::REPEAT;
					break;
				case GLFW_RELEASE:
					a = WindowKeyboardButtonEvent::Action::RELEASE;
					break;
			}

			WindowKeyboardButtonEvent e(key, a);
			w._eventManager->publish(e);
		});

		glfwSetErrorCallback([](int error, const char* description){
			LOG_ERROR("Window", "GLFW error($0): $1", error, std::string(description));
		});


		// XXX Only testing
		glfwMakeContextCurrent(_window);
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_ERROR("Window", "Failed to initialize GLAD");
			return;
		}

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		ImGui_ImplGlfw_InitForOpenGL(_window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

		LOG_VERBOSE("Window", "Window created");
	}

	Window::~Window()
	{
		glfwDestroyWindow(_window);

		if(--_glfwWindowCounter == 0)
			glfwTerminate();

		// XXX
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Window::update()
	{
		glfwPollEvents();

		// XXX Only testing
		glClearColor(0.3, 0.5, 0.8, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Demo window");
        ImGui::Button("Hello!");
        ImGui::End();

		bool demo = true;
        ImGui::ShowDemoWindow(&demo);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(_window);
	}
}
