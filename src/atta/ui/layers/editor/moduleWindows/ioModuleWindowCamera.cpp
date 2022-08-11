//--------------------------------------------------
// Atta UI Module
// ioModuleWindowCamera.cpp
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::ui {

void IOModuleWindow::cameraTabItem() {
    static auto deviceNames = io::Camera::getAvailableDeviceNames();
    std::vector<std::string> devicesWithError;

    if (ImGui::Button("Refresh"))
        deviceNames = io::Camera::getAvailableDeviceNames();

    if (ImGui::BeginTabBar("##IOModuleWindowCameraTab", ImGuiTabBarFlags_None)) {
        for (std::string& name : deviceNames) {
            // Initialize camera if not initialized yet
            if (_cameras.find(name) == _cameras.end() || _cameras[name] == nullptr) {
                io::Camera::CreateInfo info{};
                info.deviceName = name;
                info.debugName = StringId("[atta::ui] " + info.deviceName + " camera");
                _cameras[name] = io::create<io::Camera>(info);
                LOG_DEBUG("ui::IOModuleWindow", "Starting device $0, $1", name, _cameras[name] == nullptr);
                if (_cameras[name] && _cameras[name]->start()) {
                    graphics::Image::CreateInfo imgInfo{};
                    imgInfo.width = _cameras[name]->getResolution().width;
                    imgInfo.height = _cameras[name]->getResolution().height;
                    imgInfo.format = graphics::Image::Format::RGBA;
                    imgInfo.debugName = StringId("[atta::ui] " + info.deviceName + " image");
                    _cameraImages[name] = graphics::create<graphics::Image>(imgInfo);
                    LOG_DEBUG("ui::IOModuleWindow", "Camera $0 initialized", name);
                } else {
                    // Failed to initialize, remove from deviceNames
                    _cameras[name].reset();
                    _cameraImages[name].reset();
                    devicesWithError.push_back(name);
                    continue;
                }
            }

            if (_cameras[name] && ImGui::BeginTabItem((name + "##IOModuleWindowCameraTab" + name).c_str())) {
                //---------- Camera image ----------//
                // If some error occured in the camera (e.g. disconnected), stop showing
                if (!_cameras[name]->readFrame()) {
                    LOG_DEBUG("ui::IOModuleWindow", "Could not read frame, removing device", name);
                    _cameras[name].reset();
                    _cameraImages[name].reset();
                    devicesWithError.push_back(name);
                    ImGui::EndTabItem();
                    continue;
                }

                const std::vector<uint8_t> data = _cameras[name]->getFrame();
                static float cameraHeight = 200.0f;
                ImGui::BeginChild(("##imageChild" + name).c_str(), ImVec2(0, cameraHeight), true);
                {
                    if (data.size() > 0) {
                        int w, h, n;
                        char* img = (char*)stbi_load_from_memory(data.data(), data.size(), &w, &h, &n, 4);
                        _cameraImages[name]->write((void*)img);
                        stbi_image_free(img);
                        float ratio = _cameras[name]->getResolution().width / (float)_cameras[name]->getResolution().height;

                        ImVec2 avail = ImGui::GetContentRegionAvail();
                        ImVec2 window = ImGui::GetWindowSize();
                        ImVec2 size;
                        if (avail.x > avail.y * ratio) {
                            // Adjust with height
                            size = ImVec2(avail.y * ratio, avail.y);
                        } else {
                            // Adjust with width
                            size = ImVec2(avail.x, avail.x / ratio);
                        }
                        ImGui::SetCursorPos(ImVec2((window.x - size.x) * 0.5f, (window.y - avail.y) * 0.5f));
                        ImGui::Image(_cameraImages[name]->getImGuiImage(), size, ImVec2(0, 0), ImVec2(1, 1));
                    }
                }
                ImGui::EndChild();

                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                ImGui::InvisibleButton(("##heightSplitter" + name).c_str(), ImVec2(-1, 8.0f));
                if (ImGui::IsItemActive())
                    cameraHeight += ImGui::GetIO().MouseDelta.y;
                ImGui::PopStyleVar();

                ImGui::BeginChild(("##configChild" + name).c_str(), ImVec2(0, 0), true);
                {
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
                    for (auto& format : formats) {
                        // Add format
                        pixelFormats.push_back(format.pixelFormatName);
                        if (format.pixelFormat == currPixelFormat) {
                            // Add format resolutions
                            selPixelFormat = i;
                            size_t j = 0;
                            for (auto& res : format.resolutions) {
                                resolutions.push_back(std::to_string(res.width) + " x " + std::to_string(res.height));
                                // LOG_DEBUG("sf", "Res: $0 $1", res.width, res.height);
                                if (res.width == currResolution.width && res.height == currResolution.height) {
                                    for (auto f : format.fps[j])
                                        fps.push_back(std::to_string(f));
                                    selResolution = j;
                                }
                                j++;
                            }
                        }
                        i++;
                    }

                    // Pixel format combo
                    if (ImGui::BeginCombo("Pixel format", pixelFormats[selPixelFormat].c_str())) {
                        for (size_t i = 0; i < pixelFormats.size(); i++) {
                            if (ImGui::Selectable(pixelFormats[i].c_str(), i == selPixelFormat))
                                _cameras[name]->setFormat(formats[i].pixelFormat, formats[i].resolutions[0]);
                            if (i == selPixelFormat)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    // Resolution combo
                    if (ImGui::BeginCombo("Resolution", resolutions[selResolution].c_str())) {
                        for (size_t i = 0; i < resolutions.size(); i++) {
                            if (ImGui::Selectable(resolutions[i].c_str(), i == selResolution))
                                _cameras[name]->setFormat(currPixelFormat, formats[selPixelFormat].resolutions[i]);
                            if (i == selResolution)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    // Fps combo
                    if (ImGui::BeginCombo("Fps", fps[0].c_str())) {
                        for (size_t i = 0; i < fps.size(); i++) {
                            // if(ImGui::Selectable(fps[i].c_str(), i == 0));
                            //_cameras[name]->setFormat(currPixelFormat, formats[selPixelFormat].resolutions[i]);
                            if (i == 0)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }
                }
                ImGui::EndChild();

                ImGui::EndTabItem();
            }
        }
        ImGui::EndTabBar();

        // Remove devices that could not be initialized
        for (std::string deviceWithError : devicesWithError)
            for (unsigned i = 0; i < deviceNames.size(); i++)
                if (deviceNames[i] == deviceWithError) {
                    deviceNames.erase(deviceNames.begin() + i);
                    break;
                }
    }
}

} // namespace atta::ui
