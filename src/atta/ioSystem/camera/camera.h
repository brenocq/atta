//--------------------------------------------------
// Atta IO System
// camera.h
// Date: 2021-09-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_IO_SYSTEM_CAMERA_CAMERA_H
#define ATTA_IO_SYSTEM_CAMERA_CAMERA_H
#include <atta/core/stringId.h>

namespace atta::io
{
    class Camera
    {
    public:
        enum PixelFormat {
            PIXEL_FORMAT_UNKNOWN = 0,
            PIXEL_FORMAT_MJPEG,
            PIXEL_FORMAT_YUYV,
        };

        struct Resolution {
            unsigned width;
            unsigned height;
        };

        struct CreateInfo {
            std::string deviceName = "";
            PixelFormat pixelFormat = PIXEL_FORMAT_MJPEG;
            Resolution resolution = {1920, 1080};
            unsigned fps = 30;
            StringId debugName = StringId("Unnamed io::Camera");
        };

        Camera(CreateInfo info);
        virtual ~Camera() = default;

        virtual bool start() = 0;
        virtual void readFrame() = 0;
        const std::vector<uint8_t>& getFrame() const { return _frame; }
        virtual bool isValidDevice() = 0;

        //---------- Setters ----------//
        virtual bool setFormat(PixelFormat pixelFormat, Resolution resolution) = 0;
        virtual bool setFps(unsigned fps) = 0;

        //---------- Getters ----------//
        struct FormatInfo {
            PixelFormat pixelFormat;
            std::string pixelFormatName;
            std::vector<Resolution> resolutions;// Resolutions supported by each format
            std::vector<std::vector<unsigned>> fps;// Fps supported by each resolution
        };
        virtual std::vector<FormatInfo> getAvailableFormats() = 0;

        std::string getDeviceName() const { return _deviceName; }
        PixelFormat getPixelFormat() const { return _pixelFormat; }
        Resolution getResolution() const { return _resolution; }
        unsigned getFps() const { return _fps; }
        static std::vector<std::string> getAvailableDeviceNames();

    protected:
        std::string _deviceName;
        PixelFormat _pixelFormat;
        Resolution _resolution;
        unsigned _fps;
        StringId _debugName;

        std::vector<uint8_t> _frame;
    };
}

#endif// ATTA_IO_SYSTEM_CAMERA_CAMERA_H
