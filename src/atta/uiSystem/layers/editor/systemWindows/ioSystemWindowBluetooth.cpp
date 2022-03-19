//--------------------------------------------------
// Atta UI System
// ioSystemWindowBluetooth.cpp
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::ui
{
    void IOSystemWindow::bluetoothTabItem()
    {
        static bool bluetoothSupport = true;
        if(!bluetoothSupport) return;

        if(!_bluetooth)
        {
            io::Bluetooth::CreateInfo info;
            info.debugName = StringId("io::Bluetooth");
            _bluetooth = io::IOManager::create<io::Bluetooth>(info);

            if(_bluetooth)
            {
                _bluetooth->start();
                _bluetooth->startScan();
            }
            else
            {
                bluetoothSupport = false;
                return;
            }
        }
        _bluetooth->update();
        std::vector<io::Bluetooth::Device> devices = _bluetooth->getDevices();

        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | 
            ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | 
            ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
   
        ImVec2 outer_size = ImVec2(0.0f,  0.0f);
        if(ImGui::BeginTable("bluetoothDevicesTable", 4, flags, outer_size))
        {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("MAC", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("RSSI", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Connected", ImGuiTableColumnFlags_None);
            ImGui::TableHeadersRow();
   
            // Show list with clipper to support big lists
            ImGuiListClipper clipper;
            clipper.Begin(devices.size());
            while(clipper.Step())
            {
                for(int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%s", devices[i].name.c_str());
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%s", io::Bluetooth::MACToString(devices[i].mac).c_str());
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%d", devices[i].rssi);
                    ImGui::TableSetColumnIndex(3);
                    if(devices[i].connected)
                    {
                        if(ImGui::Button(("Disconnect##DisconnectBluetooth"+std::to_string(i)).c_str()))
                            _bluetooth->disconnect(devices[i].mac);
                        ImGui::SameLine();
                        if(ImGui::Button(("View##ViewBluetooth"+std::to_string(i)).c_str()))
                        {
                            // Append window if does not exist yet
                            std::array<uint8_t, 6> mac = devices[i].mac;
                            bool found = false;
                            for(auto bview : _bluetoothViews)
                                if(bview == mac)
                                    found = true;
                            if(!found)
                                _bluetoothViews.push_back(mac);
                        }
                    }
                    else
                    {
                        if(ImGui::Button(("Connect##ConnectBluetooth"+std::to_string(i)).c_str()))
                            _bluetooth->connect(devices[i].mac);
                    }
                }
            }
            ImGui::EndTable();
        }
    }

    void IOSystemWindow::bluetoothViews()
    {
        for(int i = _bluetoothViews.size()-1; i >= 0; i--)
        {
            auto mac = _bluetoothViews[i];
            std::string macStr = io::Bluetooth::MACToString(mac);
            bool open = true;
            io::Bluetooth::Device* dev = _bluetooth->getDevice(mac);

            ImGui::Begin((macStr+"##View"+macStr).c_str(), &open);
            {
                if(dev != nullptr)
                {
                    // Services
                    for(auto& serv : dev->services)
                    {
                        std::string servName  = "Service " + serv.uuid;
                        if(ImGui::TreeNode(servName.c_str()))
                        {
                            // Characteristics
                            for(auto& ch : serv.chars)
                            {
                                std::string chName  = "Char " + ch.uuid;
                                if(ImGui::TreeNode(chName.c_str()))
                                {
                                    ImGui::Text("Actions: ");
                                    if(bool(ch.flags & (io::Bluetooth::CharFlags::ANY_WRITE)))
                                    {
                                        ImGui::SameLine();
                                        ImGui::Button(("Write##Write"+ch.uuid).c_str());
                                    }
                                    if(bool(ch.flags & io::Bluetooth::CharFlags::READ))
                                    {
                                        ImGui::SameLine();
                                        ImGui::Button(("Read##Read"+ch.uuid).c_str());
                                    }
                                    if(bool(ch.flags & io::Bluetooth::CharFlags::NOTIFY))
                                    {
                                        ImGui::SameLine();
                                        ImGui::Button(("Notify##Notify"+ch.uuid).c_str());
                                    }
                                    ImGui::TreePop();
                                }
                            }
                            ImGui::TreePop();
                        }
                    }
                }
                else
                    ImGui::Text("Failed to access bluetooth device");
            }
            ImGui::End();
            if(!open)
                _bluetoothViews.erase(_bluetoothViews.begin() + i);
        }
    }
}
