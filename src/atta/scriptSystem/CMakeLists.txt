cmake_minimum_required(VERSION 3.12)

set(ATTA_SCRIPT_SYSTEM_SOURCE
    scriptManager.cpp
    script.cpp
    projectScript.cpp
)
if(NOT ATTA_STATIC_PROJECT)
    set(ATTA_SCRIPT_SYSTEM_DYNAMIC_SOURCE
        compilers/compiler.cpp
        compilers/nullCompiler.cpp
        compilers/linuxCompiler.cpp
        linkers/linker.cpp
        linkers/nullLinker.cpp
        linkers/linuxLinker.cpp
    )
    list(APPEND ATTA_SCRIPT_SYSTEM_SOURCE ${ATTA_SCRIPT_SYSTEM_DYNAMIC_SOURCE})
endif()

add_library(atta_script_system STATIC ${ATTA_SCRIPT_SYSTEM_SOURCE})
atta_target_common(atta_script_system)
atta_add_libs(atta_script_system)
