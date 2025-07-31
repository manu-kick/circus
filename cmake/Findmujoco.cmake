find_path(MUJOCO_INCLUDE_DIR mujoco/mujoco.h
    HINTS $ENV{MUJOCO_DIR}/include
)

find_library(MUJOCO_LIBRARY NAMES mujoco
    HINTS $ENV{MUJOCO_DIR}/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(mujoco REQUIRED_VARS MUJOCO_INCLUDE_DIR MUJOCO_LIBRARY)

mark_as_advanced(MUJOCO_INCLUDE_DIR MUJOCO_LIBRARY)