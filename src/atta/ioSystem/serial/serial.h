//--------------------------------------------------
// Atta IO System
// serial.h
// Date: 2021-09-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_IO_SYSTEM_SERIAL_SERIAL_H
#define ATTA_IO_SYSTEM_SERIAL_SERIAL_H
#include <atta/core/stringId.h>

namespace atta::io
{
	class Serial
	{
	public:
		struct CreateInfo {
			std::string deviceName = "";
			unsigned baudRate = 9600;
			bool parityBit = false;
			bool twoStopBits = false;
			bool rtsCts = false;
			float timeout = 1.0f;// 1 second
			StringId debugName = StringId("Unnamed io::Serial");
		};

		Serial(CreateInfo info);
		virtual ~Serial() = default;

		virtual bool start() = 0;
		virtual int receive(uint8_t* buf, uint16_t size) = 0;
		virtual void transmit(uint8_t* buf, uint16_t size) = 0;

		//---------- Setters ----------//
		virtual bool setBaudRate(unsigned baudRate) = 0;

		//---------- Getters ----------//
		std::string getDeviceName() const { return _deviceName; }
		unsigned getBaudRate() const { return _baudRate; }
		static std::vector<std::string> getAvailableDeviceNames();
		static std::vector<unsigned> getAvailableBaudRates()
		{ return {110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200, 128000, 256000 }; }

	protected:
		std::string _deviceName;
		unsigned _baudRate;
		bool _parityBit;
		bool _twoStopBits;
		bool _rtsCts;
		float _timeout;
		StringId _debugName;
	};
}

#endif// ATTA_IO_SYSTEM_SERIAL_SERIAL_H
