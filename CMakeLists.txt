cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(Tolpa)

add_subdirectory(src)
add_subdirectory(tests)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} TolpaLib)
