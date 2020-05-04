cmake_minimum_required(VERSION 2.8)

project(ball_catcher)

add_definitions(-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

message("CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DRELEASE)
endif()

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(src/common)
include_directories(src/tracking)
include_directories(src/ext)

add_executable (capture_frames
                src/capture_frames.cpp
                src/tracking/webcam.cpp
                )

add_executable (ball_tracker
                src/ball_tracker.cpp
                src/tracking/ObjectDetector.cpp
                src/tracking/webcam.cpp
                )
                

target_link_libraries( capture_frames ${OpenCV_LIBS} )
target_link_libraries( ball_tracker ${OpenCV_LIBS} )
