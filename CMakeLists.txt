cmake_minimum_required(VERSION 3.26)
project(LabProg)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(test)

set(SOURCE_FILES
        Image.h
        BaseImage.h
        BasePixel.h
        ImageFileHandler.cpp
        ImageFileHandler.h
        ImageReaderWriter.h
        KernelImageProcessing.cpp
        KernelImageProcessing.h
        Pixel.h
)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)
add_library(${CMAKE_PROJECT_NAME}_lib ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}_lib)
