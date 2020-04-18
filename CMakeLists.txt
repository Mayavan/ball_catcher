cmake_minimum_required(VERSION 2.8)

project(ball_catcher)

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

if (${CMAKE_BUILD_TYPE} STREQUAL "DEBUG")
    add_definitions(-DDEBUG)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    add_definitions(-DRELEASE)
endif()

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(common)
include_directories(tracking)

add_executable (capture_frames
                capture_frames.cpp
                tracking/webcam.cpp
                common/Settings.cpp
                )

target_link_libraries( capture_frames ${OpenCV_LIBS} )
