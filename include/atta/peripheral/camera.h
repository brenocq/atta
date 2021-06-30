//--------------------------------------------------
// Atta Peripheral
// camera.h
// Date: 2021-06-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PERIPHERAL_CAMERA_H
#define ATTA_PERIPHERAL_CAMERA_H
#include <string>
#include <vector>

namespace atta::peripheral
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
				std::string deviceName = "/dev/video0";
				PixelFormat pixelFormat = PIXEL_FORMAT_MJPEG;
				Resolution resolution = {1920, 1080};
				unsigned fps = 30;
			};

			Camera(CreateInfo createInfo);
			~Camera();

			void start();
			void readFrame();
			const std::vector<uint8_t>& getImage() const { return _image; }

			//---------- Setters ----------//
			bool setFormat(PixelFormat pixelFormat, Resolution resolution);
			bool setFps(unsigned fps);

			//---------- Getters ----------//
			struct FormatInfo {
				PixelFormat pixelFormat;
				std::string pixelFormatName;
				std::vector<Resolution> resolutions;// Resolutions supported by each format
				std::vector<std::vector<unsigned>> fps;// Fps supported by each resolution
			};
			std::vector<FormatInfo> getAvailableFormats();

		private:
			void openDevice();
			void initDevice();
			void startCapturing();

			std::string _deviceName;
			int _fd;
			PixelFormat _pixelFormat;
			Resolution _resolution;
			unsigned _fps;

			// Buffers
			struct Buffer {
				void *start;
				size_t length;
			};
			std::vector<Buffer> _buffers;
			std::vector<uint8_t> _image;
	};
}

#endif// ATTA_PERIPHERAL_CAMERA_H
