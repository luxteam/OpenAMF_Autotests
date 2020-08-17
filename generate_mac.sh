#/bin/sh

clear

rm -rf ./build
mkdir build
cd build

cmake .. -DOpenCL_INCLUDE_DIR="../../../../../OpenCL-Headers" -DAMF_CORE_STATIC="true"

cd ..