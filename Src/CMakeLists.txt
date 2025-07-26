file(GLOB_RECURSE SRC_FILES
    ${CMAKE_SOURCE_DIR}/Src/*.cpp
)

add_executable(main ${SRC_FILES})

# Link libraries
target_link_libraries(main
    mujoco
    glfw
    Eigen3::Eigen
    m
)
