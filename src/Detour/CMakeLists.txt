cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(Detour)

include_directories(../../include/Detour)

add_library(${PROJECT_NAME} STATIC
        DetourAlloc.cpp
        DetourAssert.cpp
        DetourCommon.cpp
        DetourNavMesh.cpp
        DetourNavMeshBuilder.cpp
        DetourNavMeshQuery.cpp
        DetourNode.cpp)