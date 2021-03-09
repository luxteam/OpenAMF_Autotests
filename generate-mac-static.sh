#/bin/sh

clear

rm -rf ./build
mkdir build
cd build

cmake .. -DCMAKE_BUILD_TYPE=$1 $2 -DENABLE_METAL=1 -DOpenCL_INCLUDE_DIR="../../../../../Thirdparty/OpenCL-Headers" -DAMF_CORE_STATIC="true"

cd ..