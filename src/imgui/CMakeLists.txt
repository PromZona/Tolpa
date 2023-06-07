cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(imgui)

include_directories(../../include/imgui)

add_library(${PROJECT_NAME} STATIC
        imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_tables.cpp
        imgui_widgets.cpp
        rlImGui.cpp)

target_link_libraries(${PROJECT_NAME} dwmapi raylib)
