//--------------------------------------------------
// Atta UI Module
// ioModuleWindow.h
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_EDITOR_WINDOWS_IO_MODULE_WINDOW_H
#define ATTA_UI_EDITOR_WINDOWS_IO_MODULE_WINDOW_H

#include <atta/ui/editor/window.h>

#include <atta/io/interface.h>

#include <atta/graphics/image.h>
#include <atta/graphics/interface.h>

namespace atta::ui {

class IOModuleWindow : public Window<IOModuleWindow> {
  private:
    IOModuleWindow();
    void renderImpl();

    void bluetoothTabItem();
    void bluetoothViews();

    void cameraTabItem();

    void serialTabItem();
    void serialUpdateConsole(std::string deviceName);

    // Bluetooth
    std::shared_ptr<io::Bluetooth> _bluetooth;

    struct BluetoothViewDataChar {
        std::string uuid;
        std::array<char, 20> read;
        std::array<char, 20> write;
        std::array<char, 20> notify;
    };

    struct BluetoothViewData {
        std::array<uint8_t, 6> mac;
        std::vector<BluetoothViewDataChar> chars;
    };
    std::vector<BluetoothViewData> _bluetoothViews;

    // Camera
    std::unordered_map<std::string, std::shared_ptr<io::Camera>> _cameras;
    std::unordered_map<std::string, std::shared_ptr<graphics::Image>> _cameraImages;

    // Serial
    std::unordered_map<std::string, std::shared_ptr<io::Serial>> _serials;
    std::unordered_map<std::string, std::vector<char>> _serialInputBuf;
    std::unordered_map<std::string, std::vector<char>> _serialConsoleBuf;

    friend Window<IOModuleWindow>;
};

} // namespace atta::ui

#endif // ATTA_UI_EDITOR_WINDOWS_IO_MODULE_WINDOW_H
