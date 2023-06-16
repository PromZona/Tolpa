cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(TolpaLib)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies

## RAYLIB
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
add_subdirectory(ECS)
add_subdirectory(imgui)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

add_library(${PROJECT_NAME} STATIC
        game.cpp
        EntityFactory.cpp
        InputManager.cpp
        NavigationMesh.cpp
        Renderer.cpp
        SceneManager.cpp
        KDTree.cpp
        GuiManager.cpp
        Commands/CommandManager.cpp
        Commands/CreateHumanCommand.cpp
        Commands/CreateCityCommand.cpp
        Commands/CreateOrcCommand.cpp
        Commands/CreateOrcsTribeCommand.cpp
        Controllers/MovementController.cpp
        Controllers/UnitGoalController.cpp)


# Set include directories for library
target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_link_libraries(${PROJECT_NAME} raylib ECS imgui)
