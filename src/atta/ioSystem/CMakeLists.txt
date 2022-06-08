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

set(ATTA_IO_SYSTEM_LIBS
    ${ATTA_SYSTEMD_TARGETS}
)

if(ATTA_CURLPP_SUPPORT AND (ATTA_SYSTEM_NAME MATCHES "Linux"))
    list(APPEND ATTA_IO_SYSTEM_SOURCE http/httpCurlpp.cpp)
    list(APPEND ATTA_IO_SYSTEM_LIBS ${ATTA_CURLPP_TARGETS})
elseif(ATTA_SYSTEM_NAME MATCHES "Web")
    list(APPEND ATTA_IO_SYSTEM_SOURCE http/httpWeb.cpp)
else()
    # Windows and MacOS without support yet
    list(APPEND ATTA_IO_SYSTEM_SOURCE http/httpNull.cpp)
endif()


add_library(atta_io_system STATIC ${ATTA_IO_SYSTEM_SOURCE})
target_link_libraries(atta_io_system ${ATTA_IO_SYSTEM_LIBS})

atta_target_common(atta_io_system)
atta_add_libs(atta_io_system)
