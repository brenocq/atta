set(ATTA_SYSTEMD_SUPPORT FALSE)
set(ATTA_SYSTEMD_TARGETS "")

if(ATTA_SYSTEM_NAME MATCHES "Linux")
    find_package(PkgConfig)
    if(PkgConfig_FOUND)
        pkg_search_module(LIBSYSTEMD libsystemd)
        if(LIBSYSTEMD_FOUND)
            atta_log(Success Extern "Systemd support (installed)")
            set(ATTA_SYSTEMD_SUPPORT TRUE)
            set(ATTA_SYSTEMD_TARGETS ${LIBSYSTEMD_LIBRARIES})
        else()
            atta_log(Warn Extern "Systemd not supported")
        endif()
    else()
        atta_log(Warn Extern "PkgConfig not supported. Systemd not supported")
    endif()
endif()

if(ATTA_SYSTEMD_SUPPORT)
    atta_add_definition(ATTA_SYSTEMD_SUPPORT)
endif()