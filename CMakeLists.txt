cmake_minimum_required(VERSION 3.16)
project(circus_simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

# Dependencies
find_package(Eigen3 REQUIRED)
find_package(glfw3 REQUIRED)
find_package(mujoco REQUIRED)

# headers
include_directories(
    ${MUJOCO_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/Include
)

# Subdirectory
add_subdirectory(Src)
