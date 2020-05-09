cmake_minimum_required(VERSION 2.8)

project(gpu_test)

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable (gpu_test
                gpu_test.cpp
                )
                
target_link_libraries( gpu_test ${OpenCV_LIBS} )
