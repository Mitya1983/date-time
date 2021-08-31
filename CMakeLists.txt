cmake_minimum_required(VERSION 3.17)

project(DateTime LANGUAGES CXX)
include(CustomFunctions.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(RUN_TESTS "" ON)
option(DOCS "" ON)

set_project_properties()

prepare_source_files(SRC_FILES)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

set(LIB_VERSION 1.0)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${LIB_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "date_time_${LIB_VERSION}")

if (RUN_TESTS)
    message(STATUS "Adding tests")
    add_subdirectory(Tests)
endif()

if (DOCS)
    FIND_PACKAGE(Doxygen)
    if (DOXYGEN_FOUND)
        if (NOT EXISTS ${PROJECT_BINARY_DIR}/Docs/)
            message("Creating Docs directory")
            file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/Docs/)
        endif()
        generate_doxygen_config("Simple library to handle Date and Time")
        message(STATUS "Generating API documentation")
        file(GLOB_RECURSE DOC_SOURCES
                ${PROJECT_SOURCE_DIR}/*.hpp
                )
        message(STATUS ${DOC_SOURCES})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/doxygen.conf ${CMAKE_SOURCE_DIR}/inc/date_time.hpp
    )
    else()
        message(WARNING "Doxygen not found. Documentation was not generated")
    endif(DOXYGEN_FOUND)
endif(DOCS)