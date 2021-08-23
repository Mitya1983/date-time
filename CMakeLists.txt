cmake_minimum_required(VERSION 3.17)

project(DateTime LANGUAGES CXX DESCRIPTION "Simple library to handle Date and Time")
include(CustomFunctions.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(RUN_TESTS "" ON)


set_project_properties()

prepare_source_files(SRC_FILES)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

set(LIB_VERSION 1.0)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${LIB_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "date_time_${LIB_VERSION}")

if (RUN_TESTS)
    message("Adding tests")
    add_subdirectory(Tests)
endif()
