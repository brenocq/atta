FetchContent_Declare(atta_resources
    URL "https://atta-resources.s3.amazonaws.com/v0.3.7.zip"
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources
)

atta_log(Info Extern "Downloading resources...")
FetchContent_Populate(atta_resources)
atta_log(Success Extern "Resources downloaded")
