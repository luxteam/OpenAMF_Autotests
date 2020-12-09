#include "autotests.h"

struct Metal : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFComputeFactoryPtr computeFactory;
	AMFFactory* factory;
	int deviceCount;
	AMF_RESULT res;
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("Metal");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	Metal() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		context1->GetMetalComputeFactory(&computeFactory);
		context1->InitMetal();
		g_AMFFactory.Init();
		startTime = initiateTestLog();
	}

	~Metal() {
		context1.Release();
		computeFactory.Release();
		g_AMFFactory.Terminate();
		helper.Terminate();
		terminateTestLog(startTime);
	}
};

TEST_F(Metal, AMFComputeFactory_GetDeviceAt) {
	AMFComputeDevice* device;
	EXPECT_EQ(computeFactory->GetDeviceAt(0, &device), AMF_OK);
	EXPECT_TRUE(device);
}

TEST_F(Metal, AMFDeviceCompute_CreateCompute) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	EXPECT_EQ(device->CreateCompute(nullptr, &pCompute), AMF_OK);
	EXPECT_TRUE(pCompute);
}

TEST_F(Metal, AMFDeviceCompute_CreateComputeEx) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	EXPECT_EQ(device->CreateComputeEx(nullptr, &pCompute), AMF_OK);
	EXPECT_TRUE(pCompute);
}

TEST_F(Metal, AMFCompute_GetKernel) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFPrograms* program;
	factory->GetPrograms(&program);
	AMF_KERNEL_ID kernel = 0;
	const char* kernel_src = "\n" \
		"__kernel void square2( __global float* input, __global float* output, \n" \
		" const unsigned int count) {            \n" \
		" int i = get_global_id(0);              \n" \
		" if(i < count) \n" \
		" output[i] = input[i] * input[i]; \n" \
		"}                     \n";
	program->RegisterKernelSource(&kernel, L"kernelIDName", "square2", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);
	amf::AMFComputeKernelPtr pKernel;
	EXPECT_EQ(pCompute->GetKernel(kernel, &pKernel), AMF_OK);
	EXPECT_TRUE(pKernel);
}

TEST_F(Metal, AMFCompute_PutSyncPoint) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFComputeSyncPointPtr sync;
	EXPECT_NE(pCompute->PutSyncPoint(&sync), AMF_NOT_IMPLEMENTED);
	EXPECT_TRUE(sync);
}

TEST_F(Metal, AMFCompute_FlushQueue) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	EXPECT_EQ(device->CreateCompute(nullptr, &pCompute), AMF_OK);
	EXPECT_NO_THROW(pCompute->FlushQueue());
}

TEST_F(Metal, AMFCompute_FinishQueue) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	EXPECT_NO_THROW(pCompute->FinishQueue());
}

TEST_F(Metal, DISABLED_AMFCompute_FillPlane) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_Metal, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	amf_size origin[3] = { 0, 0, 0 };
	amf_size region[3] = { 1, 1, 0 };
	float color[4] = { 1, 1, 0, 0 };
	EXPECT_NE(pCompute->FillPlane(plane, origin, region, color), AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFCompute_FillBuffer) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	EXPECT_TRUE(pCompute->GetMemoryType());
}

TEST_F(Metal, DISABLED_AMFCompute_ConvertPlaneToBuffer) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_Metal, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	AMFBufferPtr buffer;
	EXPECT_EQ(pCompute->ConvertPlaneToBuffer(plane, &buffer), AMF_OK);
	EXPECT_TRUE(buffer);
}

// TODO: Implement
TEST_F(Metal, DISABLED_AMFCompute_CopyBuffer) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	EXPECT_TRUE(pCompute->GetMemoryType());
}

// SEH exception, find null
TEST_F(Metal, DISABLED_AMFCompute_CopyPlane) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_Metal, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	AMFPlanePtr plane2;
	amf_size origin[3] = { 0, 0, 0 };
	amf_size region[3] = { 1, 1, 0 };
	float color[4] = { 1, 1, 0, 0 };
	EXPECT_EQ(pCompute->CopyPlane(plane, origin, region, plane2, origin), AMF_OK);
	EXPECT_TRUE(plane2);
}

TEST_F(Metal, AMFCompute_CopyBufferToHost) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFBufferPtr buffer;
	context1->AllocBuffer(AMF_MEMORY_Metal, 1024, &buffer);
	void* dest = malloc(1024);
	pCompute->CopyBufferToHost(buffer, 0, 1024, dest, true);
	EXPECT_TRUE(dest);
}

TEST_F(Metal, AMFCompute_CopyBufferFromHost) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFBufferPtr buffer;
	context1->AllocBuffer(AMF_MEMORY_Metal, 1024, &buffer);
	void* dest = malloc(1024);
	pCompute->CopyBufferToHost(buffer, 0, 1024, dest, true);
	AMFBufferPtr buffer2;
	context1->AllocBuffer(AMF_MEMORY_Metal, 1024, &buffer2);
	pCompute->CopyBufferFromHost(dest, 1024, buffer2, 0, true);
	EXPECT_TRUE(buffer2);
}

TEST_F(Metal, DISABLED_AMFCompute_CopyPlaneToHost) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_Metal, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	amf_size origin[3] = { 0, 0, 0 };
	amf_size region[3] = { 1, 1, 0 };
	float color[4] = { 1, 1, 0, 0 };
	void* dest = malloc(1024);
	pCompute->CopyPlaneToHost(plane, origin, region, dest, 1024, true);
	EXPECT_TRUE(dest);
}

TEST_F(Metal, DISABLED_AMFCompute_CopyPlaneFromHost) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_Metal, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	amf_size origin[3] = { 0, 0, 0 };
	amf_size region[3] = { 1, 1, 0 };
	float color[4] = { 1, 1, 0, 0 };
	void* dest = malloc(1024);
	pCompute->CopyPlaneToHost(plane, origin, region, dest, 1024, true);
	AMFPlanePtr plane2;
	pCompute->CopyPlaneFromHost(dest, origin, region, 1024, plane2, true);
	EXPECT_TRUE(plane2);
}

TEST_F(Metal, DISABLED_AMFCompute_ConvertPlaneToPlane) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_Metal, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	AMFPlanePtr plane2;
	pCompute->ConvertPlaneToPlane(plane, &plane2, AMF_CHANNEL_ORDER_R, AMF_CHANNEL_UNSIGNED_INT32);
	EXPECT_TRUE(plane2);
}

// Causes crash, needs to be investigated
TEST_F(Metal, DISABLED_AMFComputeKernel_GetCompileWorkgroupSize) {
	AMFPrograms* pPrograms;
	factory->GetPrograms(&pPrograms);
	AMF_KERNEL_ID kernel = 0;
	const char* kernel_src = "\n" \
		"__kernel void square2( __global float* output, __global float* input, \n" \
		" const unsigned int count) {            \n" \
		" int i = get_global_id(0);              \n" \
		" if(i < count) \n" \
		" output[i] = input[i] * input[i]; \n" \
		"}                     \n";
	pPrograms->RegisterKernelSource(&kernel, L"kernelIDName", "multiplication", strlen(kernel_src), (amf_uint8*)kernel_src, NULL); 
	AMFComputeDevicePtr pComputeDevice;
	computeFactory->GetDeviceAt(0, &pComputeDevice);
	pComputeDevice->GetNativeContext();

	AMFComputePtr pCompute;
	pComputeDevice->CreateCompute(nullptr, &pCompute);

	AMFComputeKernelPtr pKernel;
	pCompute->GetKernel(kernel, &pKernel);

	amf_size* size = new amf_size;
	*size = (amf_size) 64;

	res = pKernel->GetCompileWorkgroupSize(size);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFComputeKernel_Enqueue) {
	AMFPrograms* pPrograms;
	factory->GetPrograms(&pPrograms);
	AMF_KERNEL_ID kernel = 0;
	const char* kernel_src = "\n" \
		"__kernel void square2( __global float* output, __global float* input, \n" \
		" const unsigned int count) {            \n" \
		" int i = get_global_id(0);              \n" \
		" if(i < count) \n" \
		" output[i] = input[i] * input[i]; \n" \
		"}                     \n";
	pPrograms->RegisterKernelSource(&kernel, L"kernelIDName", "multiplication", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);
	AMFComputeDevicePtr pComputeDevice;
	computeFactory->GetDeviceAt(0, &pComputeDevice);
	pComputeDevice->GetNativeContext();

	AMFComputePtr pCompute;
	pComputeDevice->CreateCompute(nullptr, &pCompute);

	AMFComputeKernelPtr pKernel;
	pCompute->GetKernel(kernel, &pKernel);

	amf_size sizeLocal[3] = { 1024, 0, 0 };
	amf_size sizeGlobal[3] = { 1024, 0, 0 };
	amf_size offset[3] = { 0, 0, 0 };

	res = pKernel->Enqueue(1, offset, sizeGlobal, sizeLocal);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_Init) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_ReInit) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_Terminate) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_SubmitInput) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_QueryOutput) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_Drain) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_Flush) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_SetOutputDataAllocatorCB) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_GetCaps) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFCaps_GetInputCaps) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFCaps_GetOutputCaps) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFIOCaps_GetFormatAt) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFIOCaps_GetMemoryTypeAt) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFDataAllocatorCB_AllocBuffer) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFDataAllocatorCB_AllocSurface) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}
