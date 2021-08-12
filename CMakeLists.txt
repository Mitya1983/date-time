cmake_minimum_required(VERSION 3.17)

project(date_time LANGUAGES CXX DESCRIPTION "Simple library to handle Date and Time")

include_directories(
        inc/
        )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#TODO: Add windows and mac sections here

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "-ggdb3 -O0 -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS "-DDEBUG")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "-g0 -ggdb0 -O3")
    set(CMAKE_CXX_FLAGS "-DRELEASE")
endif(CMAKE_BUILD_TYPE STREQUAL "Release")

file (GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/inc/*.hpp
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    )

#TODO: Add shared lib build support here

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION 1.0)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER inc/date_time.h)
