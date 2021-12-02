cmake_minimum_required(VERSION 3.17)

set(PARENT_PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(PARENT_PROJECT_BINARY_DIR ${PROJECT_BINARY_DIR})
get_target_property(PARENT_OUTPUT_NAME ${PROJECT_NAME} OUTPUT_NAME)

project(Tests LANGUAGES CXX)

message(STATUS "Configuring tests")

if (NOT EXISTS ${PROJECT_SOURCE_DIR}/tests.hpp)
    file(TOUCH ${PROJECT_SOURCE_DIR}/tests.hpp)
    file(WRITE ${PROJECT_SOURCE_DIR}/tests.hpp
            "#include <gtest/gtest.h>

")
endif()
if (NOT EXISTS ${PROJECT_SOURCE_DIR}/tests.cpp)
    file(TOUCH ${PROJECT_SOURCE_DIR}/tests.cpp)
    file(WRITE ${PROJECT_SOURCE_DIR}/tests.cpp
            "#include \"tests.hpp\"

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
")
endif()

include_directories(
        ${PARENT_PROJECT_SOURCE_DIR}/inc
        /home/mitya/LibsInUse/google/googletest/include/
)

enable_testing()

add_executable(${PROJECT_NAME} tests.cpp tests.hpp)

target_link_directories(${PROJECT_NAME} PUBLIC
        /home/mitya/LibsInUse/google/googletest/
        ${PARENT_PROJECT_BINARY_DIR}/
        )
target_link_libraries(${PROJECT_NAME}
        -lgtest_main
        -lgtest
        -lpthread
        -l${PARENT_OUTPUT_NAME}
        )