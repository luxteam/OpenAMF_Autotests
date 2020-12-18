#/bin/sh
clear

rm -rf ./build
mkdir build
cd build

cmake -G "Xcode" .. -DOpenCL_INCLUDE_DIR="../../../../../Thirdparty/OpenCL-Headers" -DENABLE_METAL=1 -DAMF_CORE_STATIC="true"

cd ..