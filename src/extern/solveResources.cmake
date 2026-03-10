FetchContent_Declare(atta_resources
    URL "https://atta-resources.s3.amazonaws.com/v0.3.8.zip"
)

atta_log(Info Extern "Downloading resources...")
FetchContent_Populate(atta_resources)
atta_log(Success Extern "Resources downloaded")

# Copy to resources directory
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts")
  file(COPY "${atta_resources_SOURCE_DIR}/fonts" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/resources")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/icons")
  file(COPY "${atta_resources_SOURCE_DIR}/icons" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/resources")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/meshes")
  file(COPY "${atta_resources_SOURCE_DIR}/meshes" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/resources")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources/textures")
  file(COPY "${atta_resources_SOURCE_DIR}/textures" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/resources")
endif()

# Clean up the temporary directory
file(REMOVE_RECURSE "${atta_resources_SOURCE_DIR}")
