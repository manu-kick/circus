# circus

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
sudo apt install libmujoco2 mujoco mujoco-utils
sudo apt install qt6-base-dev qt6-base-dev-tools libqt6widgets6 libqt6openglwidgets6
```
If MuJoCo packages are not available in your distro, download and install MuJoCo manually from https://mujoco.org. Make sure to set `MUJOCO_PATH` and related environment variables accordingly.
