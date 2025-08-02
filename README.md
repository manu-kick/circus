# circus

<div align="center">
  <img src="Assets/logo.png" alt="circus Logo" width="250"/>
</div>

**circus** is a multi-platform humanoid robot soccer simulator built on top of MuJoCo. It supports mixed teams of robots, enabling realistic and flexible RoboCup simulations. Designed for research and development, circus provides accurate physics, advanced robot models, and seamless integration across operating systems.

## Build Instructions

```bash
cmake -B build -S . && cmake --build build -j$(nproc)
```

## Run

```bash
./build/Src/main
```

## Dependencies 

```bash
sudo apt update
sudo apt install qt6-base-dev qt6-base-dev-tools libqt6widgets6 libqt6openglwidgets6
sudo apt install libyaml-cpp-dev
```
If MuJoCo packages are not available in your distro, download and install MuJoCo manually from https://mujoco.org. Make sure to set `MUJOCO_PATH` and related environment variables accordingly.

### MuJoCo Installation from GitHub Release
To install MuJoCo from GitHub releases, use the mujoco_installer.sh script located in the Tools directory. The script accepts an optional version argument for the version number. If no version is specified, it will install the latest available version.

```bash
# Install latest version
./Tools/mujoco_installer.sh

# Install specific version
./Tools/mujoco_installer.sh 3.3.4
```

