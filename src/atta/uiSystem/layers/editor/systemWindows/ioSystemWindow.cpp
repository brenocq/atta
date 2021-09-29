//--------------------------------------------------
// Atta UI System
// ioSystemWindow.h
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/layers/editor/systemWindows/ioSystemWindow.h>

namespace atta::ui
{
	IOSystemWindow::IOSystemWindow()
	{
		setName("IO System");
	}

	void IOSystemWindow::renderImpl()
	{
		if(ImGui::BeginTabBar("##IOSystemWindowTab", ImGuiTabBarFlags_None))
		{
			if(ImGui::BeginTabItem("UART##IOSystemWindowUART"))
			{
				uartTabItem();
				ImGui::EndTabItem();
			}
			if(ImGui::BeginTabItem("Camera##IOSystemWindowCamera"))
			{
				cameraTabItem();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}

	void IOSystemWindow::uartTabItem()
	{
		ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
	}

#include "stb_image.h"

	void IOSystemWindow::cameraTabItem()
	{
		ImGui::Text("ID: 0123456789");
		auto deviceNames = io::IOManager::getCameraDeviceNames();

		if(_cameras.size() == 0 && deviceNames.size() > 0)
		{
			io::Camera::CreateInfo info {};
			info.deviceName = deviceNames[0];
			info.debugName = StringId("[atta::ui] "+info.deviceName+" camera");
			_cameras.push_back(io::IOManager::create<io::Camera>(info));
			_cameras.back()->start();

			Image::CreateInfo imgInfo {};
			imgInfo.width = _cameras.back()->getResolution().width;
			imgInfo.height = _cameras.back()->getResolution().height;
			imgInfo.format = Image::Format::RGBA;
			imgInfo.debugName = StringId("[atta::ui] "+info.deviceName+" image");
			_cameraImages.push_back(GraphicsManager::create<Image>(imgInfo));
		}

		for(uint32_t i = 0; i < _cameras.size(); i++)
		{
			_cameras[i]->readFrame();
			const std::vector<uint8_t> data = _cameras[i]->getFrame();
			int w, h, n;
			char* img = (char*)stbi_load_from_memory(data.data(), data.size(), &w, &h, &n, 4);

			_cameraImages[i]->write((void*)img);
			ImGui::Separator();
			float ratio = _cameras[i]->getResolution().width/(float)_cameras[i]->getResolution().height;
			ImVec2 size = ImVec2(200*ratio,200);
			ImGui::Image(_cameraImages[i]->getImGuiImage(), size, ImVec2(0, 0), ImVec2(1, 1));
		}
	}
}
