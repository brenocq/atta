//--------------------------------------------------
// Robot Simulator
// userInterface.cpp
// Date: 10/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "userInterface.h"

UserInterface::UserInterface(Device* device, Window* window, SwapChain* swapChain, Scene* scene):
	_showPhysicsDebugger(false),
	// Toggle variables
   	_splitRender(nullptr), _enableRayTracing(nullptr)
{
	//---------- Get main objects ----------//
	_device = device;
	_window = window;
	_swapChain = swapChain;
	_scene = scene;
	PhysicalDevice* physicalDevice = _device->getPhysicalDevice();
	Instance* instance = physicalDevice->getInstance();

	//---------- Create ui objects ----------//
	createDescriptorPool();
	_imguiRenderPass = new UiRenderPass(_device, _swapChain);
	_imguiCommandPool = new CommandPool(_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

	_imguiFrameBuffers.resize(_swapChain->getImageViews().size());
	for(size_t i = 0; i < _imguiFrameBuffers.size(); i++) 
		_imguiFrameBuffers[i] = new UiFrameBuffer(_swapChain->getImageViews()[i], _imguiRenderPass);
	createCommandBuffers();

	//---------- Init imgui ----------//
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	if(!ImGui_ImplGlfw_InitForVulkan(_window->handle(), true))
	{
		std::cout << BOLDRED << "[UserInterface]" << RESET << RED << " Failed to initialise ImGui GLFW adapter!" << RESET << std::endl;
		exit(1);
	}

    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = instance->handle();
    initInfo.PhysicalDevice = physicalDevice->handle();
    initInfo.Device = _device->handle();
    initInfo.QueueFamily = 0;// TODO not being used
    initInfo.Queue = _device->getGraphicsQueue();
    initInfo.PipelineCache = VK_NULL_HANDLE;
    initInfo.DescriptorPool = _imguiDescriptorPool->handle();
    initInfo.Allocator = nullptr;
    initInfo.MinImageCount = _swapChain->getImages().size();
    initInfo.ImageCount = _swapChain->getImages().size();
    initInfo.CheckVkResultFn = UserInterface::checkResult;

    if(!ImGui_ImplVulkan_Init(&initInfo, _imguiRenderPass->handle()))
	{
		std::cout << BOLDRED << "[UserInterface]" << RESET << RED << " Failed to initialise ImGui VULKAN adapter!" << RESET << std::endl;
		exit(1);
	}
	
	//---------- Upload fonts ----------//
	VkCommandBuffer command_buffer = _imguiCommandPool->beginSingleTimeCommands();
	if(!ImGui_ImplVulkan_CreateFontsTexture(command_buffer))
	{
		std::cout << BOLDRED << "[UserInterface]" << RESET << RED << " Failed to create fonts texture!" << RESET << std::endl;
		exit(1);
	}
	_imguiCommandPool->endSingleTimeCommands(command_buffer);

	//ImGui_ImplVulkan_DestroyFontUploadObjects();
	ImGui_ImplVulkan_SetMinImageCount(_swapChain->getImages().size());

	//---------- Init UI variables ---------//
}

UserInterface::~UserInterface()
{
	ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	for (auto frameBuffer : _imguiFrameBuffers) 
	{
		delete frameBuffer;
		frameBuffer = nullptr;
    }

	vkFreeCommandBuffers(_device->handle(), _imguiCommandPool->handle(), static_cast<uint32_t>(_imguiCommandBuffers.size()), _imguiCommandBuffers.data());

	delete _imguiCommandPool;
	_imguiCommandPool = nullptr;

	delete _imguiRenderPass;
	_imguiRenderPass = nullptr;

	delete _imguiDescriptorPool;
	_imguiDescriptorPool = nullptr;
}

void UserInterface::createDescriptorPool()
{
	std::vector<VkDescriptorPoolSize> poolSizes =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };
	_imguiDescriptorPool = new DescriptorPool(_device, poolSizes);
}

void UserInterface::createCommandBuffers()
{
	// TODO put it in a beautiful way
	_imguiCommandBuffers.resize(_imguiFrameBuffers.size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = _imguiCommandPool->handle();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t) _imguiCommandBuffers.size();

	if(vkAllocateCommandBuffers(_device->handle(), &allocInfo, _imguiCommandBuffers.data()) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[UserInterface]" << RESET << RED << " Failed to allocate command buffers!" << RESET << std::endl;
		exit(1);
	}
}

void UserInterface::checkResult(VkResult result)
{
	if(result != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[UserInterface]" << RESET << RED << " Failed to setup Imgui info!" << RESET << std::endl;
		exit(1);
	}
}


void UserInterface::render(int i)
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	draw();
	ImGui::Render();

	//---------- Begin command buffer ----------//
	VkCommandBufferBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(_imguiCommandBuffers[i], &info);

	//---------- User interface render pass ----------//
	VkRenderPassBeginInfo uiRenderPassInfo = {};
	uiRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	uiRenderPassInfo.renderPass = _imguiRenderPass->handle();
	uiRenderPassInfo.framebuffer = _imguiFrameBuffers[i]->handle();
	uiRenderPassInfo.renderArea.extent = _swapChain->getExtent();
	uiRenderPassInfo.clearValueCount = 1;
	VkClearValue clearValueUi{};
	clearValueUi.color = {0.0f, 0.0f, 0.0f, 1.0f};
	uiRenderPassInfo.pClearValues = &clearValueUi;
	vkCmdBeginRenderPass(_imguiCommandBuffers[i], &uiRenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	{
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), _imguiCommandBuffers[i]);
	}
	vkCmdEndRenderPass(_imguiCommandBuffers[i]);
	vkEndCommandBuffer(_imguiCommandBuffers[i]);
}

//---------------------------------------------//
//-------------- DRAW FUNCTIONS ---------------//
//---------------------------------------------//

void UserInterface::draw()
{
	ImGui::ShowDemoWindow();
	static bool showScene = true;

	if(ImGui::BeginMainMenuBar())
	{
        if(ImGui::BeginMenu("Main"))
        {
			if(_splitRender != nullptr)
            	ImGui::Checkbox("Split Render", _splitRender);
			if(_enableRayTracing != nullptr)
            	ImGui::Checkbox("RayTracing (r)", _enableRayTracing);
            if(ImGui::MenuItem("Quit", "ESQ")) {}
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("Window"))
        {
            if(ImGui::Checkbox("Scene", &showScene)) {}
            ImGui::EndMenu();
        }

        if(ImGui::BeginMenu("View"))
        {
            if(ImGui::Checkbox("Physics debugger", &_showPhysicsDebugger)) {}
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
	}
	
	if(showScene) showSceneWindow(&showScene);
}

//---------------------------------------------//
//----------------- IMGUI main ----------------//
//---------------------------------------------//

void UserInterface::showSceneWindow(bool* showWindow)
{
	static bool showObjectInfo = false;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->GetWorkPos());
	ImGui::SetNextWindowSize(ImVec2(200,viewport->GetWorkSize().y));

  	ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize;
        //ImGuiWindowFlags_NoCollapse |
        //ImGuiWindowFlags_NoSavedSetting;
        //ImGuiWindowFlags_AlwaysAutoResize |
        //ImGuiWindowFlags_NoTitleBar;

	// Check scene tree and process clicks
	ImGui::Begin("Scene", showWindow, flags);
	{
        ImGui::Text("Scene tree");ImGui::SameLine();helpMarker("Right-click tree node to show object menu");

		ImGui::BeginChild("World Objects", ImVec2(0, 0), true);
		{
			for(auto object : _scene->getObjects())
				if(object->getParent() == nullptr)
					createSceneTreeNode(object);
		}
        ImGui::EndChild();
	}
	ImGui::End();

	showObjectInfoWindows();
}

void UserInterface::showObjectInfoWindows()
{
	for(auto object : _scene->getObjects())
	{
		if(!_showObjectInfo[object])
			continue;

		ImGui::SetNextWindowSize(ImVec2(300, 300));

		ImGuiWindowFlags flags =
			ImGuiWindowFlags_NoResize;

		ImGui::Begin(object->getName().c_str(), &_showObjectInfo[object], flags);
		{
			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if(ImGui::BeginTabBar("ObjectInfoTabBar", tab_bar_flags))
			{
				if(ImGui::BeginTabItem("Transformation"))
				{
					//bool isStatic = object->getStatic();
					//ImGui::Checkbox("Static", &isStatic);
					//object->setStatic(isStatic);
					if(ImGui::TreeNode("Position"))
					{
						float posX = object->getPosition().x;
						float posY = object->getPosition().y;
						float posZ = object->getPosition().z;
            			ImGui::SliderFloat("X (m)", &posX, -10, 10, "%.3f");
            			ImGui::SliderFloat("Y (m)", &posY, -10, 10, "%.3f");
            			ImGui::SliderFloat("Z (m)", &posZ, -10, 10, "%.3f");
	
						object->setPosition({posX, posY, posZ});

						ImGui::TreePop();
						ImGui::Separator();
					}
					if(ImGui::TreeNode("Rotation"))
					{
						float rotX = object->getRotation().x;
						float rotY = object->getRotation().y;
						float rotZ = object->getRotation().z;
            			ImGui::SliderFloat("X (degree)", &rotX, 0, 360, "%.3f");
            			ImGui::SliderFloat("Y (degree)", &rotY, 0, 360, "%.3f");
            			ImGui::SliderFloat("Z (degree)", &rotZ, 0, 360, "%.3f");
	
						object->setRotation({rotX, rotY, rotZ});

						ImGui::TreePop();
						ImGui::Separator();
					}
					ImGui::EndTabItem();
				}
				if(ImGui::BeginTabItem("Properties"))
				{
					ImGui::BulletText("Object type: %s", object->getType().c_str());
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		ImGui::End();
	}
}

//---------------------------------------------//
//--------------- IMGUI helpers ---------------//
//---------------------------------------------//

void UserInterface::helpMarker(std::string text)
{
    ImGui::TextDisabled("(?)");
    if(ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::TextUnformatted(text.c_str());
        ImGui::EndTooltip();
    }
}

// Scene tree
void UserInterface::createSceneTreeNode(Object* object)
{
	std::string objectName = object->getName();
	std::vector<Object*> children = object->getChildren();

	if(children.size() == 0)
	{
		// Leaf node
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
		ImGui::TreeNodeEx(objectName.c_str(), nodeFlags, objectName.c_str());
		objectMenuPopup(object);
	}
	else
	{
		// Tree node
		if(ImGui::TreeNode(objectName.c_str()))
		{
			objectMenuPopup(object);
			for(auto child : children)
			{
				createSceneTreeNode(child);
			}
			ImGui::TreePop();
		}
		else
		{
			objectMenuPopup(object);
		}
	}
}

// Object menu
void UserInterface::objectMenuPopup(Object* object)
{
	std::string menuPopupName = object->getName() + " popup";
	if(ImGui::BeginPopupContextItem(menuPopupName.c_str()))
	{
		if(ImGui::Selectable("Edit"))
		{
			_showObjectInfo[object] = true;
		}
		if(ImGui::Selectable("Delete"))
		{
			// TODO
		}
		ImGui::EndPopup();
	}
}

