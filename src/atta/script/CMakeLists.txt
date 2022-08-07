cmake_minimum_required(VERSION 3.12)

set(ATTA_SCRIPT_MODULE_SOURCE
    scriptManager.cpp
    script.cpp
    projectScript.cpp
)
if(NOT ATTA_STATIC_PROJECT)
    set(ATTA_SCRIPT_MODULE_DYNAMIC_SOURCE
        compilers/compiler.cpp
        compilers/nullCompiler.cpp
        compilers/linuxCompiler.cpp
        linkers/linker.cpp
        linkers/nullLinker.cpp
        linkers/linuxLinker.cpp
    )
    list(APPEND ATTA_SCRIPT_MODULE_SOURCE ${ATTA_SCRIPT_MODULE_DYNAMIC_SOURCE})
endif()

add_library(atta_script_module STATIC ${ATTA_SCRIPT_MODULE_SOURCE})
atta_target_common(atta_script_module)
atta_add_libs(atta_script_module)
