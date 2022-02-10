cmake_minimum_required(VERSION 3.12)

set(ATTA_IO_SYSTEM_SOURCE
    ioManager.cpp

    camera/camera.cpp
    camera/linuxCamera.cpp

    serial/serial.cpp
    serial/linuxSerial.cpp

    http/http.cpp
    http/httpCppRestSdk.cpp
)

add_library(atta_io_system STATIC ${ATTA_IO_SYSTEM_SOURCE})
atta_target_common(atta_io_system)
atta_add_libs(atta_io_system)
