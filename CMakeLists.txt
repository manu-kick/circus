cmake_minimum_required(VERSION 3.16)
project(spqr_simulator)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Eigen3 REQUIRED)
find_package(glfw3 REQUIRED)
find_package(console_bridge REQUIRED)

set(MUJOCO_PATH /home/brock/mujoco-3.3.0) # needed since I don't have the pre-compiled version of Mujoco
set(MUJOCO_INCLUDE_DIR ${MUJOCO_PATH}/include)
set(MUJOCO_LIBRARY ${MUJOCO_PATH}/lib/libmujoco.so)

include_directories(${MUJOCO_INCLUDE_DIR})
link_directories(${MUJOCO_PATH}/lib)

add_subdirectory(Src)