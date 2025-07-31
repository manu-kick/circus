cmake_minimum_required(VERSION 3.16)
project(circus_simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

# Dependencies
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(mujoco REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

# headers
include_directories(
    ${MUJOCO_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/Include
)

# Subdirectory
add_subdirectory(Src)
