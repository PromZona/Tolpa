cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(Tolpa)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 4.5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

# Our Project

include_directories(include)

# Create a list of your CPP source files using globbing
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

# Create a list of your header files using globbing
file(GLOB_RECURSE HEADER_FILES include/*.hpp)

# Create your executable target and link the source and header files
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)