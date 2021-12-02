cmake_minimum_required(VERSION 3.17)

project(DateTime LANGUAGES CXX)
include(CustomFunctions.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(DOCS "" ON)

set_project_properties()

prepare_source_files(SRC_FILES)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})
add_library(${PROJECT_NAME}_Shared SHARED ${SRC_FILES})

foreach (FILE ${SRC_FILES})
    string(REGEX MATCHALL ".*cpp" IS_CPP ${FILE})
    if (IS_CPP)
        list(REMOVE_ITEM SRC_FILES "${FILE}")
    endif ()
endforeach ()

set(LIB_VERSION 1.0)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${LIB_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME}_Shared PROPERTIES VERSION ${LIB_VERSION})
set_target_properties(${PROJECT_NAME}_Shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

add_subdirectory(Tests)

if (DOCS)
    FIND_PACKAGE(Doxygen REQUIRED doxygen)
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Docs)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_SORT_MEMBER_DOCS NO)
    doxygen_add_docs(
            doxygen
            ${PROJECT_SOURCE_DIR}/inc
            ALL
    )
endif (DOCS)

install(TARGETS ${PROJECT_NAME}
        CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        DESTINATION ${CMAKE_BUILD_TYPE}/lib/static)
install(TARGETS ${PROJECT_NAME}_Shared
        CONFIGURATIONS ${CMAKE_BUILD_TYPE}
        DESTINATION ${CMAKE_BUILD_TYPE}/lib/shared)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/inc DESTINATION ${CMAKE_BUILD_TYPE})
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src DESTINATION ${CMAKE_BUILD_TYPE})

