#/bin/sh

rm -rf ./linux
mkdir linux

cd linux
cmake .. -DCMAKE_BUILD_TYPE=$1 -DOpenCL_INCLUDE_DIR="../../../../../Thirdparty/OpenCL-Headers" -DAMF_CORE_SHARED=1
cd ..