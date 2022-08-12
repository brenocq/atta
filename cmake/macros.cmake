macro(atta_add_libs)
    # Used by atta systems to register itself to be linked with the atta target
    foreach(_lib ${ARGN})
        list(APPEND ATTA_LIBS ${_lib})
    endforeach()

    # Update variable
    get_directory_property(hasParent PARENT_DIRECTORY)
    if(hasParent)
        set(ATTA_LIBS ${ATTA_LIBS} PARENT_SCOPE)
    else()
        set(ATTA_LIBS ${ATTA_LIBS})
    endif()
endmacro()

macro(atta_add_include_dirs)
    # Used by atta systems to include directories to all targets
    foreach(_include_dir ${ARGN})
        list(APPEND ATTA_INCLUDE_DIRS ${_include_dir})
    endforeach()

    # Update variable
    get_directory_property(hasParent PARENT_DIRECTORY)
    if(hasParent)
        set(ATTA_INCLUDE_DIRS ${ATTA_INCLUDE_DIRS} PARENT_SCOPE)
    else()
        set(ATTA_INCLUDE_DIRS ${ATTA_INCLUDE_DIRS})
    endif()
endmacro()

macro(atta_add_tests)
    # Used by atta systems to add tests to the atta_test target
    foreach(_test ${ARGN})
        list(APPEND ATTA_TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${_test}")
    endforeach()

    # Update variable
    get_directory_property(hasParent PARENT_DIRECTORY)
    if(hasParent)
        set(ATTA_TEST_SOURCES ${ATTA_TEST_SOURCES} PARENT_SCOPE)
    else()
        set(ATTA_TEST_SOURCES ${ATTA_TEST_SOURCES})
    endif()
endmacro()

macro(atta_add_definition)
    # Used to add a define when compiling
    foreach(_definition ${ARGN})
        list(APPEND ATTA_DEFINITIONS ${_definition})
    endforeach()

    # Update variable
    get_directory_property(hasParent PARENT_DIRECTORY)
    if(hasParent)
        set(ATTA_DEFINITIONS ${ATTA_DEFINITIONS} PARENT_SCOPE)
    else()
        set(ATTA_DEFINITIONS ${ATTA_DEFINITIONS})
    endif()
endmacro()

macro(atta_target_common target)
    # Common to all targets
    target_precompile_headers(${target} PRIVATE ${ATTA_PCH})
    target_include_directories(${target} PRIVATE ${ATTA_INCLUDE_DIRS})
    target_compile_options(${target} PRIVATE ${ATTA_OPTIONS})
    target_compile_definitions(${target} PRIVATE ${ATTA_DEFINITIONS})
endmacro()

macro(atta_create_local_test target sources libs)
if(ATTA_BUILD_TESTS)
    add_executable(${target} ${sources})
    atta_target_common(${target})
    target_link_libraries(${target} PRIVATE ${libs} gtest_main)
    set_target_properties(${target} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ATTA_TEST_PATH})
endif()
endmacro()
