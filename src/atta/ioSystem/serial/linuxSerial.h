//--------------------------------------------------
// Atta IO System
// linuxSerial.h
// Date: 2021-09-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_IO_SYSTEM_SERIAL_LINUX_SERIAL_H
#define ATTA_IO_SYSTEM_SERIAL_LINUX_SERIAL_H
#ifdef ATTA_OS_LINUX
#include <atta/core/stringId.h>
#include <atta/ioSystem/serial/serial.h>
#include <termios.h>

namespace atta::io
{
	class LinuxSerial : public Serial
	{
	public:
		LinuxSerial(Serial::CreateInfo info);
		~LinuxSerial();

		bool start() override;
		int receive(uint8_t* buf, uint16_t size) override;
		void transmit(uint8_t* buf, uint16_t size) override;

		//---------- Setters ----------//
		bool setBaudRate(unsigned baudRate) override;

	private:
		int _fd;
		struct termios _tty;
	};
}
#endif// ATTA_OS_LINUX
#endif// ATTA_IO_SYSTEM_SERIAL_LINUX_SERIAL_H
