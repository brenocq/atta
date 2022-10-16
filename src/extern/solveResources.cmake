FetchContent_Declare(resources
    URL "http://storage.googleapis.com/atta-resources/atta/resources-0.4.0.0.zip"
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources
)

atta_log(Info Extern "Downloading resources...")
FetchContent_Populate(resources)
atta_log(Success Extern "Resources downloaded")
