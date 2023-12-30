//--------------------------------------------------
// Atta UI Module
// manager.cpp
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/interface.h>
#include <atta/ui/manager.h>

// clang-format off
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#if ATTA_VULKAN_SUPPORT
#include <backends/imgui_impl_vulkan.h>
#include <atta/graphics/apis/vulkan/vulkanAPI.h>
#endif
#include <ImGuizmo.h>
#include <implot.h>
// clang-format on

namespace atta::ui {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUpImpl() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking

    // Don't save imgui.ini
    io.IniFilename = NULL;
#ifdef ATTA_OS_WEB
    // Disable file-system access when building with Emscripten (no fopen() of the imgui.init file)
    io.IniFilename = NULL;
#else
    // Multiple viewports not supported for the web
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport
#endif

    switch (gfx::getGraphicsAPI()->getType()) {
        case gfx::GraphicsAPI::OPENGL:
            initOpenGL();
            break;
        case gfx::GraphicsAPI::VULKAN:
            initVulkan();
            break;
    }
    setTheme();

    gfx::setUiRenderFunc([&]() { render(); });
    gfx::setUiShutDownFunc([&]() { shutDownImpl(); });
    gfx::setUiStartUpFunc([&]() { startUpImpl(); });

    // Create editor (make sure windows are reorganized after start up)
    _editor = {};
}

void Manager::shutDownImpl() {
    // Make sure all rendering operations are done
    gfx::getGraphicsAPI()->waitDevice();

    switch (gfx::getGraphicsAPI()->getType()) {
        case gfx::GraphicsAPI::OPENGL:
            ImGui_ImplOpenGL3_Shutdown();
            break;
        case gfx::GraphicsAPI::VULKAN:
#if ATTA_VULKAN_SUPPORT
            ImGui_ImplVulkan_Shutdown();
#endif
            break;
    }
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Manager::setTheme() {
    ImGui::StyleColorsDark();
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

    // Headers
    colors[ImGuiCol_Header] = ImVec4{0.25f, 0.255f, 0.26f, 1.0f};
    colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
    colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
    colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
}

void Manager::initOpenGL() {
    GLFWwindow* window = (GLFWwindow*)gfx::getWindow()->getHandle();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 100");
}

void Manager::initVulkan() {
#if ATTA_VULKAN_SUPPORT
    GLFWwindow* window = (GLFWwindow*)gfx::getWindow()->getHandle();
    ImGui_ImplGlfw_InitForVulkan(window, true);
    std::shared_ptr<gfx::VulkanAPI> vulkanAPI = std::dynamic_pointer_cast<gfx::VulkanAPI>(gfx::getGraphicsAPI());
    ImGui_ImplVulkan_InitInfo info{};
    info.Instance = vulkanAPI->getInstance()->getHandle();
    info.PhysicalDevice = vulkanAPI->getPhysicalDevice()->getHandle();
    info.Device = vulkanAPI->getDevice()->getHandle();
    info.QueueFamily = vulkanAPI->getDevice()->getGraphicsQueueFamily();
    info.Queue = vulkanAPI->getDevice()->getGraphicsQueue();
    info.PipelineCache = nullptr;
    info.DescriptorPool = vulkanAPI->getUiDescriptorPool()->getHandle();
    info.Allocator = nullptr;
    info.MinImageCount = 3;
    info.ImageCount = 3;
    info.CheckVkResultFn = nullptr;
    ImGui_ImplVulkan_Init(&info, vulkanAPI->getRenderPass()->getHandle());

    // Upload Fonts
    if (!ImGui_ImplVulkan_CreateFontsTexture())
        LOG_WARN("ui::Manager", "Failed to created ImGui font texture");
#endif
}

void Manager::render() {
    // Begin
    switch (gfx::getGraphicsAPI()->getType()) {
        case gfx::GraphicsAPI::OPENGL:
            ImGui_ImplOpenGL3_NewFrame();
            break;
        case gfx::GraphicsAPI::VULKAN:
#if ATTA_VULKAN_SUPPORT
            ImGui_ImplVulkan_NewFrame();
#endif
            break;
    }
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    // Render UI
    _editor.render();

    // End
    ImGui::Render();
    switch (gfx::getGraphicsAPI()->getType()) {
        case gfx::GraphicsAPI::OPENGL:
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            break;
        case gfx::GraphicsAPI::VULKAN:
#if ATTA_VULKAN_SUPPORT
            ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),
                                            std::dynamic_pointer_cast<gfx::VulkanAPI>(gfx::getGraphicsAPI())->getCommandBuffers()->getCurrent());
#endif
            break;
    }

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);
    }
}

} // namespace atta::ui
