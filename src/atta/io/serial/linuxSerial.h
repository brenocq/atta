// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#ifdef ATTA_OS_LINUX
#include <asm/termbits.h>
#include <atta/io/serial/serial.h>
#include <atta/utils/stringId.h>

namespace atta::io {

class LinuxSerial final : public Serial {
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
    struct termios2 _tty;
};

} // namespace atta::io
#endif // ATTA_OS_LINUX
