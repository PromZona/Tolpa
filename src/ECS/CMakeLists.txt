cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(ECS)

add_library(${PROJECT_NAME} ECS.cpp)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

target_link_libraries(${PROJECT_NAME} raylib)