//--------------------------------------------------
// Atta UI Module
// ioSystemWindowSerial.cpp
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::ui {

void IOSystemWindow::serialTabItem() {
    auto deviceNames = io::Serial::getAvailableDeviceNames();

    // Remove disconnected devices
repeatSerialErase:
    for (const auto& [key, value] : _serials) {
        bool found = std::find(deviceNames.begin(), deviceNames.end(), key) != deviceNames.end();
        if (!found) {
            _serials.erase(key);
            _serialInputBuf.erase(key);
            goto repeatSerialErase;
        }
    }

    if (ImGui::BeginTabBar("##IOSystemWindowSerialTab", ImGuiTabBarFlags_None)) {
        for (auto& name : deviceNames) {
            // Initialize if it is first time
            if (_serials.find(name) == _serials.end()) {
                io::Serial::CreateInfo info{};
                info.deviceName = name;
                info.baudRate = 9600;
                info.timeout = 0.0f;
                info.debugName = StringId("[atta::ui] " + info.deviceName + " serial");
                _serials[name] = io::Manager::create<io::Serial>(info);
                _serials[name]->start();

                _serialInputBuf[name].resize(1024);
                _serialConsoleBuf[name] = {'\0'};
            }

            // Serial console tab
            if (ImGui::BeginTabItem((name + "##IOSystemWindowSerialTab" + name).c_str())) {
                //---------- Input ----------//
                bool reclaimFocus = false;
                ImGuiInputTextFlags inputTextFlags = ImGuiInputTextFlags_EnterReturnsTrue;
                if (ImGui::InputText(("##Input" + name).c_str(), _serialInputBuf[name].data(), _serialInputBuf[name].size(), inputTextFlags)) {
                    // Write to serial when enter pressed
                    _serials[name]->transmit((uint8_t*)_serialInputBuf[name].data(), strlen(_serialInputBuf[name].data()));
                    _serialInputBuf[name][0] = '\0';
                    reclaimFocus = true;
                }
                ImGui::SetItemDefaultFocus();
                if (reclaimFocus)
                    ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

                ImGui::SameLine();
                if (ImGui::Button(("Send##SendButton" + name).c_str())) {
                    // Write to serial when button pressed
                    _serials[name]->transmit((uint8_t*)_serialInputBuf[name].data(), strlen(_serialInputBuf[name].data()));
                    _serialInputBuf[name][0] = '\0';
                }

                // Console text
                const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
                ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);

                ImGui::TextUnformatted(_serialConsoleBuf[name].data());

                ImGui::EndChild();
                ImGui::Separator();

                // Clear button
                if (ImGui::Button(("Clear##Clear" + name).c_str())) {
                    _serialConsoleBuf[name].clear();
                    _serialConsoleBuf[name].push_back('\0');
                }
                ImGui::SameLine();

                // Baud rate combo
                auto baudRates = io::Serial::getAvailableBaudRates();

                std::vector<std::string> baudRateStr;
                for (auto baudRate : baudRates)
                    baudRateStr.push_back(std::to_string(baudRate));

                int selBaudRate = 0;
                int currBaudRate = _serials[name]->getBaudRate();
                for (unsigned i = 0; i < baudRates.size(); i++)
                    if ((int)baudRates[i] == currBaudRate)
                        selBaudRate = (int)i;

                if (ImGui::BeginCombo("Baud rate", baudRateStr[selBaudRate].c_str())) {
                    for (unsigned i = 0; i < baudRates.size(); i++) {
                        if (ImGui::Selectable(baudRateStr[i].c_str(), i == (unsigned)selBaudRate))
                            _serials[name]->setBaudRate(baudRates[i]);
                        if ((int)i == selBaudRate)
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

void IOSystemWindow::serialUpdateConsole(std::string deviceName) {
    // Update console every 0.01 seconds
    const float updateTime = 0.1f;
    static clock_t lastTime = std::clock();
    const clock_t currTime = std::clock();

    float timeDiff = float(currTime - lastTime) / CLOCKS_PER_SEC;
    if (timeDiff > updateTime) {
        lastTime = currTime;

        // Read char by char
        int count = 1;
        char buf[256];
        while (count > 0) {
            count = _serials[deviceName]->receive((uint8_t*)&buf, sizeof(buf));
            if (count > 0) {
                _serialConsoleBuf[deviceName].pop_back();                                                    // Remove \0
                _serialConsoleBuf[deviceName].insert(_serialConsoleBuf[deviceName].end(), buf, buf + count); // Add chars
                _serialConsoleBuf[deviceName].push_back('\0');                                               // Add \0
            }
        }
    }
}

} // namespace atta::ui
