add_executable(main
  main.cpp
  Simulator.cpp
)

set(COMMON_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}
  ${EIGEN3_INCLUDE_DIR}
)

target_include_directories(main
  PUBLIC
    ${COMMON_INCLUDE_DIRS}
)

target_link_libraries(main
  glfw
  ${MUJOCO_LIBRARY}
)

set_target_properties(main PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
