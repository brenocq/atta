cmake_minimum_required(VERSION 3.12)

set(ATTA_IO_MODULE_SOURCE
    ioManager.cpp

    camera/camera.cpp
    camera/linuxCamera.cpp

    serial/serial.cpp
    serial/linuxSerial.cpp

    bluetooth/bluetooth.cpp
    bluetooth/linuxBluetooth.cpp

    # http implementation is chosen based on the operating system
    http/json.cpp
)

set(ATTA_IO_MODULE_LIBS
    ${ATTA_SYSTEMD_TARGETS}
)

if(ATTA_CURLPP_SUPPORT AND (ATTA_MODULE_NAME MATCHES "Linux"))
    list(APPEND ATTA_IO_MODULE_SOURCE http/httpCurlpp.cpp)
    list(APPEND ATTA_IO_MODULE_LIBS ${ATTA_CURLPP_TARGETS})
elseif(ATTA_MODULE_NAME MATCHES "Web")
    list(APPEND ATTA_IO_MODULE_SOURCE http/httpWeb.cpp)
else()
    # Windows and MacOS without support yet
    list(APPEND ATTA_IO_MODULE_SOURCE http/httpNull.cpp)
endif()


add_library(atta_io_module STATIC ${ATTA_IO_MODULE_SOURCE})
target_link_libraries(atta_io_module ${ATTA_IO_MODULE_LIBS})

atta_target_common(atta_io_module)
atta_add_libs(atta_io_module)

########## Testing ##########
set(ATTA_IO_MODULE_TEST_SOURCES
    tests/json.cpp
)
# Add to global test
atta_add_tests(${ATTA_IO_MODULE_TEST_SOURCES})

# Create local test
atta_create_local_test(
    atta_io_module_test
    "${ATTA_IO_MODULE_TEST_SOURCES}"
    "atta_io_module"
)
