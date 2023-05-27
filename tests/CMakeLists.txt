cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(Testing)

set(GTEST_VERSION 4.5.0)
find_package(googletest ${GTEST_VERSION} QUIET) # QUIET or REQUIRED
if (NOT googletest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        b796f7d44681514f58a683a3a71ff17c94edb0c1
    )
    FetchContent_GetProperties(googletest)
    if (NOT googletest_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(googletest)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    endif()
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(${PROJECT_NAME}
        main_test.cpp
        ComponentVector_test.cpp
        ECS_test.cpp)
target_link_libraries(
        ${PROJECT_NAME}
        GTest::gtest_main
        raylib
        TolpaLib
)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../include)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})