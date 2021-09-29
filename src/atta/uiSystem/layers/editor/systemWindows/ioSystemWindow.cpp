//--------------------------------------------------
// Atta UI System
// ioSystemWindow.h
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/layers/editor/systemWindows/ioSystemWindow.h>
#include "stb_image.h"

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

	void IOSystemWindow::cameraTabItem()
	{
		auto deviceNames = io::Camera::getAvailableDeviceNames();

		if(ImGui::BeginTabBar("##IOSystemWindowCameraTab", ImGuiTabBarFlags_None))
		{
			for(auto& name : deviceNames)
			{
				if(ImGui::BeginTabItem((name+"##IOSystemWindowCameraTab"+name).c_str()))
				{
					// Initialize camera if not initialized yet
					if(_cameras.find(name) == _cameras.end())
					{
						io::Camera::CreateInfo info {};
						info.deviceName = name;
						info.debugName = StringId("[atta::ui] "+info.deviceName+" camera");
						_cameras[name] = io::IOManager::create<io::Camera>(info);
						_cameras[name]->start();

						Image::CreateInfo imgInfo {};
						imgInfo.width = _cameras[name]->getResolution().width;
						imgInfo.height = _cameras[name]->getResolution().height;
						imgInfo.format = Image::Format::RGBA;
						imgInfo.debugName = StringId("[atta::ui] "+info.deviceName+" image");
						_cameraImages[name] = GraphicsManager::create<Image>(imgInfo);
					}

					//---------- Camera image ----------//
					_cameras[name]->readFrame();
					const std::vector<uint8_t> data = _cameras[name]->getFrame();
					int w, h, n;
					char* img = (char*)stbi_load_from_memory(data.data(), data.size(), &w, &h, &n, 4);

					_cameraImages[name]->write((void*)img);
					float ratio = _cameras[name]->getResolution().width/(float)_cameras[name]->getResolution().height;
					ImVec2 size = ImVec2(200*ratio,200);
					ImGui::Image(_cameraImages[name]->getImGuiImage(), size, ImVec2(0, 0), ImVec2(1, 1));
							ImGui::EndTabItem();

					//---------- Camera config ----------//
					ImGui::Separator();
					std::vector<io::Camera::FormatInfo> formats = _cameras[name]->getAvailableFormats();
					io::Camera::PixelFormat currPixelFormat = _cameras[name]->getPixelFormat();
					io::Camera::Resolution currResolution = _cameras[name]->getResolution();

					unsigned selPixelFormat = 0;
					unsigned selResolution = 0;
					std::vector<std::string> pixelFormats;
					std::vector<std::string> resolutions;
					std::vector<std::string> fps;

					size_t i = 0;
					for(auto& format : formats)
					{
						// Add format
						pixelFormats.push_back(format.pixelFormatName);
						if(format.pixelFormat == currPixelFormat)
						{
							// Add format resolutions
							selPixelFormat = i;
							size_t j = 0;
							for(auto& res : format.resolutions)
							{
								resolutions.push_back(
										std::to_string(res.width) + 
										" x " + 
										std::to_string(res.height));
								//LOG_DEBUG("sf", "Res: $0 $1", res.width, res.height);
								if(res.width == currResolution.width && res.height == currResolution.height)
								{
									for(auto f : format.fps[j])
										fps.push_back(std::to_string(f));
									selResolution = j;
								}
								j++;
							}
						}
						i++;
					}

					// Pixel format combo
					if(ImGui::BeginCombo("Pixel format", pixelFormats[selPixelFormat].c_str()))
					{
						for(size_t i = 0; i < pixelFormats.size(); i++)
						{
							if(ImGui::Selectable(pixelFormats[i].c_str(), i == selPixelFormat))
								_cameras[name]->setFormat(formats[i].pixelFormat, formats[i].resolutions[0]);
							if(i == selPixelFormat)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					// Resolution combo
					if(ImGui::BeginCombo("Resolution", resolutions[selResolution].c_str()))
					{
						for(size_t i = 0; i < resolutions.size(); i++)
						{
							if(ImGui::Selectable(resolutions[i].c_str(), i == selResolution))
								_cameras[name]->setFormat(currPixelFormat, formats[selPixelFormat].resolutions[i]);
							if(i == selResolution)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					// Fps combo
					if(ImGui::BeginCombo("Fps", fps[0].c_str()))
					{
						for(size_t i = 0; i < fps.size(); i++)
						{
							if(ImGui::Selectable(fps[i].c_str(), i == 0));
								//_cameras[name]->setFormat(currPixelFormat, formats[selPixelFormat].resolutions[i]);
							if(i == 0)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}
				}
			}
			ImGui::EndTabBar();
		}
	}
}
