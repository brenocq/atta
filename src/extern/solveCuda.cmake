set(ATTA_CUDA_SUPPORT FALSE)
set(ATTA_CUDA_TARGETS "")

find_package(CUDA)
if(CUDA_FOUND)
    set(ATTA_CUDA_SUPPORT TRUE)
    set(ATTA_CUDA_TARGETS ${CUDA_LIBRARIES})

    # Register CUDA directory
    atta_add_include_dirs(${CUDA_INCLUDE_DIRS})
    atta_log(Success Extern "CUDA support (installed)")

    # Configure CUDA compilation
    #set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -include ${ATTA_PCH}")
    set(CMAKE_CUDA_ARCHITECTURES 60)# TODO hardcoded
else()
    atta_log(Error Extern "CUDA not supported, please install the CUDA Toolkit if you want to use the GPU device")
endif()
