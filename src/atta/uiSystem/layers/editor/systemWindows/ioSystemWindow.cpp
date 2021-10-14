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
            if(ImGui::BeginTabItem("Serial##IOSystemWindowSerial"))
            {
                serialTabItem();
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

    void IOSystemWindow::serialTabItem()
    {
        auto deviceNames = io::Serial::getAvailableDeviceNames();

        // Remove disconnected devices
    repeatSerialErase:
        for(const auto& [key, value] : _serials)
        {
            bool found = std::find(deviceNames.begin(), deviceNames.end(), key) != deviceNames.end();
            if(!found)
            {
                _serials.erase(key);
                _serialInputBuf.erase(key);
                goto repeatSerialErase;
            }
        }

        if(ImGui::BeginTabBar("##IOSystemWindowSerialTab", ImGuiTabBarFlags_None))
        {
            for(auto& name : deviceNames)
            {
                // Initialize if it is first time
                if(_serials.find(name) == _serials.end())
                {
                    io::Serial::CreateInfo info {};
                    info.deviceName = name;
                    info.baudRate = 115200;
                    info.timeout = 0.0f;
                    info.debugName = StringId("[atta::ui] "+info.deviceName+" serial");
                    _serials[name] = io::IOManager::create<io::Serial>(info);
                    _serials[name]->start();

                    _serialInputBuf[name].resize(1024);
                    _serialConsoleBuf[name] = { '\0' };
                }

                // Serial console tab
                if(ImGui::BeginTabItem((name+"##IOSystemWindowSerialTab"+name).c_str()))
                {
                    //---------- Input ----------//
                    bool reclaimFocus = false;
                    ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue;
                    if(ImGui::InputText(("##Input"+name).c_str(), 
                                _serialInputBuf[name].data(), _serialInputBuf[name].size(), 
                                inputTextFlags))
                    {
                        // Write to serial when enter pressed
                        _serials[name]->transmit((uint8_t*)_serialInputBuf[name].data(), strlen(_serialInputBuf[name].data())+1);
                        _serialInputBuf[name][0] = '\0';
                        reclaimFocus = true;
                    }
                    ImGui::SetItemDefaultFocus();
                    if(reclaimFocus)
                        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

                    ImGui::SameLine();
                    if(ImGui::Button(("Send##SendButton"+name).c_str()))
                    {
                        // Write to serial when button pressed
                        _serials[name]->transmit((uint8_t*)_serialInputBuf[name].data(), strlen(_serialInputBuf[name].data())+1);
                        _serialInputBuf[name][0] = '\0';
                    }

                    // Console text
                    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
                    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

                    ImGui::TextUnformatted(_serialConsoleBuf[name].data());

                    ImGui::EndChild();
                    ImGui::Separator();

                    // Clear button
                    if(ImGui::Button(("Clear##Clear"+name).c_str()))
                    {
                        _serialConsoleBuf[name].clear();
                        _serialConsoleBuf[name].push_back('\0');
                    }
                    ImGui::SameLine();

                    // Baud rate combo
                    auto baudRates = io::Serial::getAvailableBaudRates();

                    std::vector<std::string> baudRateStr;
                    for(auto baudRate : baudRates)
                        baudRateStr.push_back(std::to_string(baudRate));

                    int selBaudRate = 0;
                    int currBaudRate = _serials[name]->getBaudRate();
                    for(unsigned i = 0; i < baudRates.size(); i++)
                        if(baudRates[i] == currBaudRate) selBaudRate = i;

                    if(ImGui::BeginCombo("Baud rate", baudRateStr[selBaudRate].c_str()))
                    {
                        for(size_t i = 0; i < baudRates.size(); i++)
                        {
                            if(ImGui::Selectable(baudRateStr[i].c_str(), i == selBaudRate))
                                _serials[name]->setBaudRate(baudRates[i]);
                            if(i == selBaudRate)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::EndTabItem();
                }

                // Update console characters with data from serial
                serialUpdateConsole(name);
            }
            ImGui::EndTabBar();
        }
    }

    void IOSystemWindow::serialUpdateConsole(std::string deviceName)
    {
        // Update console every 0.01 seconds
        const float updateTime = 0.1f;
        static clock_t lastTime = std::clock();
        const clock_t currTime = std::clock();

        float timeDiff = float(currTime-lastTime)/CLOCKS_PER_SEC;
        if(timeDiff > updateTime)
        {
            lastTime = currTime;

            // Read char by char
            int count = 1;
            char buf[256];
            while(count > 0)
            {
                count = _serials[deviceName]->receive((uint8_t*)&buf, sizeof(buf));
                if(count > 0)
                {
                    _serialConsoleBuf[deviceName].pop_back();// Remove \0
                    _serialConsoleBuf[deviceName].insert(_serialConsoleBuf[deviceName].end(), buf, buf+count);// Add chars
                    _serialConsoleBuf[deviceName].push_back('\0');// Add \0
                }
            }
        }
    }

    void IOSystemWindow::cameraTabItem()
    {
        auto deviceNames = io::Camera::getAvailableDeviceNames();

        if(ImGui::BeginTabBar("##IOSystemWindowCameraTab", ImGuiTabBarFlags_None))
        {
            for(auto& name : deviceNames)
            {
                // Initialize camera if not initialized yet
                if(_cameras.find(name) == _cameras.end() || _cameras[name] == nullptr)
                {
                    io::Camera::CreateInfo info {};
                    info.deviceName = name;
                    info.debugName = StringId("[atta::ui] "+info.deviceName+" camera");
                    _cameras[name] = io::IOManager::create<io::Camera>(info);
                    if(_cameras[name]->start())
                    {
                        Image::CreateInfo imgInfo {};
                        imgInfo.width = _cameras[name]->getResolution().width;
                        imgInfo.height = _cameras[name]->getResolution().height;
                        imgInfo.format = Image::Format::RGBA;
                        imgInfo.debugName = StringId("[atta::ui] "+info.deviceName+" image");
                        _cameraImages[name] = GraphicsManager::create<Image>(imgInfo);
                    }
                    else
                        _cameras[name].reset();
                }

                if(_cameras[name] && ImGui::BeginTabItem((name+"##IOSystemWindowCameraTab"+name).c_str()))
                {
                    //---------- Camera image ----------//
                    _cameras[name]->readFrame();
                    const std::vector<uint8_t> data = _cameras[name]->getFrame();
                    int w, h, n;
                    char* img = (char*)stbi_load_from_memory(data.data(), data.size(), &w, &h, &n, 4);

                    _cameraImages[name]->write((void*)img);
                    float ratio = _cameras[name]->getResolution().width/(float)_cameras[name]->getResolution().height;
                    ImVec2 size = ImVec2(200*ratio,200);
                    ImGui::Image(_cameraImages[name]->getImGuiImage(), size, ImVec2(0, 0), ImVec2(1, 1));

                    //---------- Camera config ----------//
                    ImGui::Separator();
                    std::vector<io::Camera::FormatInfo> formats = _cameras[name]->getAvailableFormats();
                    io::Camera::PixelFormat currPixelFormat = _cameras[name]->getPixelFormat();
                    io::Camera::Resolution currResolution = _cameras[name]->getResolution();

                    // If some error occured in the camera (e.g. disconnected), stop showing
                    if(formats.size() == 0)
                    {
                        _cameras[name].reset();
                        _cameraImages[name].reset();
                        ImGui::EndTabItem();
                        continue;
                    }

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

                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }
    }
}
