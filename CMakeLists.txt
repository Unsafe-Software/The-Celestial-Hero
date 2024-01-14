cmake_minimum_required(VERSION 3.8.0)
project(The-Celestial-Hero VERSION 0.0.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)

# --- Config ---
configure_file(
  "${PROJECT_SOURCE_DIR}/cfg/config.in.hpp"
  "${PROJECT_SOURCE_DIR}/src/config.hpp"
)

# --- Assets ---
file(
  COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

# --- Libraries ---
include(FetchContent)
# Raylib
FetchContent_Declare(
  raylib
  GIT_REPOSITORY "https://github.com/raysan5/raylib"
  GIT_TAG "5.0"
)
FetchContent_MakeAvailable(raylib)

# --- Executables ---
# Conf atom
add_executable("confs" atoms/conf.c)
# Game
file(GLOB Sources src/*.cpp src/gfx/*.cpp src/world/*.cpp src/physics/*.cpp)
add_compile_definitions("RELEASE=$<CONFIG:Release>")
add_executable("${PROJECT_NAME}" ${Sources})
target_link_libraries("${PROJECT_NAME}" raylib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPACK)
