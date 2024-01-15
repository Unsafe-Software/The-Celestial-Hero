cmake_minimum_required(VERSION 3.8.0)
project(The-Celestial-Hero VERSION 0.0.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 11)

# --- Data ---
file(
  COPY ${CMAKE_CURRENT_SOURCE_DIR}/data
  DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

# --- Libraries ---
include(FetchContent)
# Raylib
FetchContent_Declare(
  raylib
  GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
  GIT_TAG "5.0"
)
FetchContent_MakeAvailable(raylib)
# JSON
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY "https://github.com/jbeder/yaml-cpp.git"
  GIT_TAG "0.8.0"
)
FetchContent_GetProperties(yaml-cpp)
if(NOT yaml-cpp_POPULATED)
  message(STATUS "Fetching yaml-cpp...")
  FetchContent_Populate(yaml-cpp)
  add_subdirectory(${yaml-cpp_SOURCE_DIR} ${yaml-cpp_BINARY_DIR})
endif()

# --- Executables ---
# Game
file(GLOB Sources src/*.cpp src/gfx/*.cpp src/world/*.cpp src/entities/*.cpp)
add_compile_definitions("RELEASE=$<CONFIG:Release>")
add_executable("${PROJECT_NAME}" ${Sources})
target_link_libraries("${PROJECT_NAME}" raylib yaml-cpp::yaml-cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPACK)
