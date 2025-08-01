cmake_minimum_required(VERSION 3.16)
project(circus_simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

# Dependencies
set(MUJOCO_ROOT "${CMAKE_SOURCE_DIR}/Libs/mujoco")

add_library(mujoco SHARED IMPORTED GLOBAL)
set_target_properties(mujoco PROPERTIES
    IMPORTED_LOCATION "${MUJOCO_ROOT}/lib/libmujoco.so"
)
target_include_directories(mujoco INTERFACE "${MUJOCO_ROOT}/include")

find_package(OpenGL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)

# Headers
include_directories(
    ${CMAKE_SOURCE_DIR}/Include
)

# Subdirectory
add_subdirectory(Src)
