#!/bin/bash
BUILD_DIR="build"
EXE="main"

clear

[ -d "$BUILD_DIR" ] || cmake -S . -B "$BUILD_DIR"

cmake --build "$BUILD_DIR" -j"$(nproc)"

./"$BUILD_DIR/$EXE"
