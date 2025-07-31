set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE SRC_FILES
    ${CMAKE_SOURCE_DIR}/Src/*.cpp
    ${CMAKE_SOURCE_DIR}/Include/*.h # It must be listed explicitly for AUTOMOC
)

add_executable(main ${SRC_FILES})

# Link libraries
# target_link_libraries(main
#     mujoco
#     Qt6::Widgets
#     Qt6::OpenGLWidgets
#     m
# )

target_link_libraries(main 
    PRIVATE
    ${MUJOCO_LIBRARY} 
    OpenGL::GL 
    Qt6::Widgets 
    Qt6::OpenGLWidgets
)
