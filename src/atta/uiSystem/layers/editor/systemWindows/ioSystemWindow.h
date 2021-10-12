//--------------------------------------------------
// Atta UI System
// ioSystemWindow.h
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOWS_IO_SYSTEM_WINDOW_H
#define ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOWS_IO_SYSTEM_WINDOW_H
#include <atta/uiSystem/layers/editor/window.h>

#include <atta/ioSystem/ioManager.h>

#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/graphicsSystem/image.h>

namespace atta::ui
{
    class IOSystemWindow : public Window<IOSystemWindow>
    {
    private:
        IOSystemWindow();
        void renderImpl();

        void serialTabItem();
        void serialUpdateConsole(std::string deviceName);

        void cameraTabItem();

        // Serial
        std::unordered_map<std::string, std::shared_ptr<io::Serial>> _serials;
        std::unordered_map<std::string, std::vector<char>> _serialInputBuf;
        std::unordered_map<std::string, std::vector<char>> _serialConsoleBuf;

        // Camera
        std::unordered_map<std::string, std::shared_ptr<io::Camera>> _cameras;
        std::unordered_map<std::string, std::shared_ptr<Image>> _cameraImages;

        friend Window<IOSystemWindow>;
    };
}

#endif// ATTA_UI_SYSTEM_LAYERS_EDITOR_WINDOWS_IO_SYSTEM_WINDOW_H
