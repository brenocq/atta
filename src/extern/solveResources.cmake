FetchContent_Declare(resources
    URL "https://atta-resources.s3.amazonaws.com/v0.3.2.zip"
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources
)

atta_log(Info Extern "Downloading resources...")
FetchContent_Populate(resources)
atta_log(Success Extern "Resources downloaded")
