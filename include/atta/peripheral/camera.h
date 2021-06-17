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
			struct CreateInfo {
				std::string deviceName = "/dev/video0";
			};

			Camera(CreateInfo createInfo);
			~Camera();

			void readFrame();
			const std::vector<uint8_t>& getImage() const { return _image; }

		private:
			void openDevice();
			void initDevice();
			void startCapturing();

			std::string _deviceName;
			int _fd;

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
