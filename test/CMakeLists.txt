cmake_minimum_required(VERSION 3.26)

set(CXX_STANDARD 17)

set(PATH_TO_GOOGLETEST ./lib/googletest/)
add_subdirectory(${PATH_TO_GOOGLETEST})
include_directories(${PATH_TO_GOOGLETEST}/googletest/include)

add_executable(runImageTests runAllTests.cpp
        ImageFixture.cpp
        PixelFixture.cpp
        KIPTest.cpp
        ImageTest.cpp
)
target_link_libraries(runImageTests gtest gtest_main)