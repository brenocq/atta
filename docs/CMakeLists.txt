set(ATTA_DOXYGEN_MARKDOWN
    "docs/overview.md"
    "docs/hello.md"
    "docs/references.md"
)
list(APPEND ATTA_DOXYGEN_SOURCES ${ATTA_DOXYGEN_MARKDOWN})
file(GLOB_RECURSE ATTA_DOXYGEN_HEADERS "../src/atta/*.h")
foreach(header IN LISTS ATTA_DOXYGEN_HEADERS)
	file(RELATIVE_PATH relHeader ${CMAKE_SOURCE_DIR} ${header})
	list(APPEND ATTA_DOXYGEN_SOURCES ${relHeader})
endforeach()

# Format the source list into a Doxyfile INPUT value that Doxygen can parse
foreach(path IN LISTS ATTA_DOXYGEN_SOURCES)
	set(ATTA_DOXYGEN_INPUT "${ATTA_DOXYGEN_INPUT} \\\n\"${CMAKE_SOURCE_DIR}/${path}\"")
endforeach()

# https://cmake.org/cmake/help/latest/command/configure_file.html
configure_file(Doxyfile.in Doxyfile @ONLY)

add_custom_target(docs ALL "${DOXYGEN_EXECUTABLE}"
                  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/docs"
                  COMMENT "Generating HTML documentation" VERBATIM)

install(
  DIRECTORY "${CMAKE_BINARY_DIR}/docs/html"
  DESTINATION "${CMAKE_INSTALL_DOCDIR}"
)
