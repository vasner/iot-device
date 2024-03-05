#!/bin/bash

# Runs iot-device host tests

set -e

BUILD_DIR="build"
rm -rf "${BUILD_DIR}"
mkdir "${BUILD_DIR}"
cd "${BUILD_DIR}"
cmake ..
cd ..
cmake --build "./${BUILD_DIR}"

cd "${BUILD_DIR}"
./iot_host

