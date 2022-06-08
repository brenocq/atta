cmake_minimum_required(VERSION 3.12)

set(ATTA_IO_SYSTEM_SOURCE
    ioManager.cpp

    camera/camera.cpp
    camera/linuxCamera.cpp

    serial/serial.cpp
    serial/linuxSerial.cpp

    bluetooth/bluetooth.cpp
    bluetooth/linuxBluetooth.cpp

    # http implementation is chosen based on the operating system
)

if(ATTA_CPPNETLIB_SUPPORT AND (ATTA_SYSTEM_NAME MATCHES "Linux"))
    list(APPEND ATTA_IO_SYSTEM_SOURCE http/httpCppNetlib.cpp)
elseif(ATTA_SYSTEM_NAME MATCHES "Web")
    list(APPEND ATTA_IO_SYSTEM_SOURCE http/httpWeb.cpp)
else()
    # Windows and MacOS without support yet
    list(APPEND ATTA_IO_SYSTEM_SOURCE http/httpNull.cpp)
endif()


add_library(atta_io_system STATIC ${ATTA_IO_SYSTEM_SOURCE})
target_link_libraries(atta_io_system ${ATTA_SYSTEMD_TARGETS} ${ATTA_CPPNETLIB_TARGETS})

atta_target_common(atta_io_system)
atta_add_libs(atta_io_system)
