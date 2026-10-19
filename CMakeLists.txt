cmake_minimum_required(VERSION 4.0)
project(kernel_img_processing)

set(CMAKE_CXX_STANDARD 20)

add_executable(kernel_img_processing src/main.cpp
        include/image_io.cpp
        include/image_io.h
        include/filters.h
        include/filters.cpp)

target_include_directories(kernel_img_processing PRIVATE include)