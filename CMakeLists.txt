cmake_minimum_required(VERSION 3.28)
project(YOBA-SFML-full-example LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main
add_executable(${PROJECT_NAME}
  src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# SFML
include(FetchContent)
FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 3.1.0
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL
  SYSTEM
)
FetchContent_MakeAvailable(SFML)

target_link_libraries(${PROJECT_NAME} PUBLIC SFML::Graphics)

# YOBA
add_subdirectory(lib/YOBA)
target_link_libraries(${PROJECT_NAME} PRIVATE YOBA)