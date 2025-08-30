# ============================= PROJECT SETUP =================================================
cmake_minimum_required(VERSION 3.22)
project(circus VERSION 0.1 LANGUAGES CXX)
set(CIRCUS_CPP_LIBRARY ${PROJECT_NAME})
set(CIRCUS_PY_LIBRARY "${PROJECT_NAME}py")
set(PROJECT circus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(cxx_setup INTERFACE)
target_compile_options(cxx_setup INTERFACE -Wall -Wpedantic -Wextra)
target_compile_features(cxx_setup INTERFACE cxx_std_17)
add_compile_definitions(PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

if(CMAKE_CXX_COMPILER_LOADED)
  message(STATUS "Path: ${CMAKE_CXX_COMPILER}")
  message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
  message(STATUS "Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
# =============================================================================================


# ============================ BUILD OPTIONS ==================================================
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_PYTHON_BINDINGS "Build python bindings" ON)
option(GENERATE_CLANGD "Generate .clangd in project root" ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions("DEBUG")
endif()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
# =============================================================================================

# ============================= Dependencies ==================================================

set(MUJOCO_ROOT "${CMAKE_SOURCE_DIR}/Libs/mujoco")
add_library(mujoco SHARED IMPORTED GLOBAL)
set_target_properties(mujoco PROPERTIES
    IMPORTED_LOCATION "${MUJOCO_ROOT}/lib/libmujoco.so"
)
target_include_directories(mujoco INTERFACE "${MUJOCO_ROOT}/include")
add_subdirectory(Libs/pugixml)

# TODO: Dirty solution
file(GLOB HOST_DEP_ROOT_LIST RELATIVE "${CMAKE_SOURCE_DIR}" ".pixi/build/work/circus*/host*")
list(GET HOST_DEP_ROOT_LIST 0 HOST_DEP_ROOT_REL)
cmake_path(APPEND HOST_DEP_ROOT ${CMAKE_SOURCE_DIR} ${HOST_DEP_ROOT_REL})
file(COPY "/usr/include/GL" DESTINATION "${HOST_DEP_ROOT}/include")

find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
# ==============================================================================================


# ============================ CIRCUS LIBRARY ==================================================
set(CMAKE_AUTOMOC ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
file(GLOB_RECURSE CIRCUS_SRCS ${CMAKE_SOURCE_DIR}/Src/*.cpp)
list(REMOVE_ITEM CIRCUS_SRCS ${CMAKE_SOURCE_DIR}/Src/Bindings.cpp)
file(GLOB_RECURSE CIRCUS_HDRS ${CMAKE_SOURCE_DIR}/Include/*.h)
add_library(${CIRCUS_CPP_LIBRARY} STATIC ${CIRCUS_SRCS})
target_sources(${CIRCUS_CPP_LIBRARY} PUBLIC ${CIRCUS_HDRS})
set_target_properties(${CIRCUS_CPP_LIBRARY} PROPERTIES PUBLIC_HEADER "${CIRCUS_HDRS}")

target_include_directories(${CIRCUS_CPP_LIBRARY}
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Include>
  $<BUILD_INTERFACE:${eigen_SOURCE_DIR}>
  $<BUILD_INTERFACE:${MUJOCO_ROOT}/include>
)

set(CIRCUS_CPP_EXTRA_LIBRARIES
  cxx_setup
  mujoco
  pugixml
  yaml-cpp
  Qt6::Widgets
  Qt6::OpenGLWidgets
  Eigen3::Eigen
)

target_link_libraries(${CIRCUS_CPP_LIBRARY} PUBLIC ${CIRCUS_CPP_EXTRA_LIBRARIES})
add_executable(main Src/main.cpp)
target_link_libraries(main PRIVATE ${CIRCUS_CPP_LIBRARY} mujoco)
# ==============================================================================================

# ============================= PYTHON BINDINGS ================================================
find_package(Python 3.8 COMPONENTS Interpreter Development.Module REQUIRED)

execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE nanobind_ROOT
)
find_package(nanobind CONFIG REQUIRED)

include_directories(${python_INCLUDE_DIRS})
execute_process(
    COMMAND ${Python_EXECUTABLE} -c "import sysconfig; print(sysconfig.get_path('purelib'))"
    OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

nanobind_add_module(${CIRCUS_PY_LIBRARY} ${PROJECT_SOURCE_DIR}/Src/Bindings.cpp)
target_link_libraries(${CIRCUS_PY_LIBRARY} PUBLIC ${CIRCUS_CPP_LIBRARY})

install(
    TARGETS ${CIRCUS_PY_LIBRARY}
    EXPORT ${CIRCUS_PY_LIBRARY}Targets
    LIBRARY DESTINATION ${PYTHON_SITE_PACKAGES}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${BINDIR}
)
# ===============================================================================================


# ============================== CLANGD =========================================================
if(GENERATE_CLANGD)
  file(GLOB BUILD_ROOT_LIST RELATIVE "${CMAKE_SOURCE_DIR}" ".pixi/build/work/circus*/work/build")
  list(GET BUILD_ROOT_LIST 0 BUILD_ROOT_REL)
  cmake_path(APPEND BUILD_ROOT ${CMAKE_SOURCE_DIR} ${BUILD_ROOT_REL})
  file(WRITE "${CMAKE_SOURCE_DIR}/.clangd"
    "CompileFlags:\n  CompilationDatabase: ${BUILD_ROOT}\n")
endif()
# ===============================================================================================
