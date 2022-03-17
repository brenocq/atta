set(ATTA_SYSTEMD_SUPPORT FALSE)
set(ATTA_SYSTEMD_TARGETS "")

if(ATTA_SYSTEM_NAME MATCHES "Linux")
    find_package(PkgConfig REQUIRED)
    pkg_search_module(LIBSYSTEMD REQUIRED libsystemd)

    atta_log(Success Extern "Systemd support (installed)")
    set(ATTA_SYSTEMD_SUPPORT TRUE)
    set(ATTA_SYSTEMD_TARGETS ${LIBSYSTEMD_LIBRARIES})
endif()
