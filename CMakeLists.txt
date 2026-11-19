cmake_minimum_required(VERSION 4.0)
project(kernel_img_processing)

set(CMAKE_CXX_STANDARD 20)

include(CheckLanguage)
check_language(CUDA)

if(CMAKE_CUDA_COMPILER)
    enable_language(CUDA)
    set(SOURCES
        src/main.cpp
        include/image_io.cpp
        include/filters.cpp
        cuda/filters_cuda.cu
    )
else ()
    set(SOURCES
            src/main.cpp
            include/image_io.cpp
            include/filters.cpp
            cuda/filters_cuda.cu
    )
    set_source_files_properties(cuda/filters_cuda.cu PROPERTIES LANGUAGE CXX)
endif ()

add_executable(kernel_img_processing ${SOURCES})
target_include_directories(kernel_img_processing PRIVATE include)