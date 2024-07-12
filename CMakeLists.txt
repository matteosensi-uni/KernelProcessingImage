cmake_minimum_required(VERSION 3.26)
project(LabProg)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(test)

set(SOURCE_FILES
        Image.h
        KernelImageProcessing.h
        Pixel.h
)

add_executable(${CMAKE_PROJECT_NAME} main.cpp
)
