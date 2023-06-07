cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(Recast)

include_directories(../../include/Recast)

add_library(${PROJECT_NAME} STATIC
        Recast.cpp
        RecastAlloc.cpp
        RecastArea.cpp
        RecastAssert.cpp
        RecastContour.cpp
        RecastFilter.cpp
        RecastLayers.cpp
        RecastMesh.cpp
        RecastMeshDetail.cpp
        RecastRasterization.cpp
        RecastRegion.cpp)