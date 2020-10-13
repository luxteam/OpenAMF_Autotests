#include "autotests.h"

// AMFComputeFactory

struct Metal_AMFComputeFactory : testing::Test {
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

	Metal_AMFComputeFactory() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		context1->GetMetalComputeFactory(&computeFactory);
		context1->InitMetal();
		g_AMFFactory.Init();
		startTime = initiateTestLog();
	}

	~Metal_AMFComputeFactory() {
		context1.Release();
		computeFactory.Release();
		g_AMFFactory.Terminate();
		helper.Terminate();
		terminateTestLog(startTime);
	}
};

TEST_F(Metal_AMFComputeFactory, AMFComputeFactory_GetDeviceAt) {
	AMFComputeDevice* device;
	EXPECT_EQ(computeFactory->GetDeviceAt(0, &device), AMF_OK);
	EXPECT_TRUE(device);
}

// AMFDeviceCompute

struct Metal_AMFDeviceCompute : testing::Test {
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

	Metal_AMFDeviceCompute() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		context1->GetMetalComputeFactory(&computeFactory);
		context1->InitMetal();
		g_AMFFactory.Init();
		startTime = initiateTestLog();
	}

	~Metal_AMFDeviceCompute() {
		context1.Release();
		computeFactory.Release();
		g_AMFFactory.Terminate();
		helper.Terminate();
		terminateTestLog(startTime);
	}
};

TEST_F(Metal_AMFDeviceCompute, CreateCompute) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	EXPECT_EQ(device->CreateCompute(nullptr, &pCompute), AMF_OK);
	EXPECT_TRUE(pCompute);
}

TEST_F(Metal_AMFDeviceCompute, CreateComputeEx) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	EXPECT_EQ(device->CreateComputeEx(nullptr, &pCompute), AMF_OK);
	EXPECT_TRUE(pCompute);
}

// AMFCompute

struct Metal_AMFCompute : testing::Test {
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

	Metal_AMFCompute() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		context1->GetMetalComputeFactory(&computeFactory);
		context1->InitMetal();
		g_AMFFactory.Init();
		startTime = initiateTestLog();
	}

	~Metal_AMFCompute() {
		context1.Release();
		computeFactory.Release();
		g_AMFFactory.Terminate();
		helper.Terminate();
		terminateTestLog(startTime);
	}
};

TEST_F(Metal_AMFCompute, GetKernel) {
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

TEST_F(Metal_AMFCompute, PutSyncPoint) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFComputeSyncPointPtr sync;
	EXPECT_NE(pCompute->PutSyncPoint(&sync), AMF_NOT_IMPLEMENTED);
	EXPECT_TRUE(sync);
}

TEST_F(Metal_AMFCompute, FlushQueue) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	EXPECT_EQ(device->CreateCompute(nullptr, &pCompute), AMF_OK);
	EXPECT_NO_THROW(pCompute->FlushQueue());
}

TEST_F(Metal_AMFCompute, FinishQueue) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	EXPECT_NO_THROW(pCompute->FinishQueue());
}

TEST_F(Metal_AMFCompute, DISABLED_FillPlane) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_METAL, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	amf_size origin[3] = { 0, 0, 0 };
	amf_size region[3] = { 1, 1, 0 };
	float color[4] = { 1, 1, 0, 0 };
	EXPECT_NE(pCompute->FillPlane(plane, origin, region, color), AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal_AMFCompute, FillBuffer) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	EXPECT_TRUE(pCompute->GetMemoryType());
}

TEST_F(Metal_AMFCompute, DISABLED_ConvertPlaneToBuffer) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_METAL, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	AMFBufferPtr buffer;
	EXPECT_EQ(pCompute->ConvertPlaneToBuffer(plane, &buffer), AMF_OK);
	EXPECT_TRUE(buffer);
}

// TODO: Implement
TEST_F(Metal_AMFCompute, DISABLED_CopyBuffer) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	EXPECT_TRUE(pCompute->GetMemoryType());
}

// SEH exception, find null
TEST_F(Metal_AMFCompute, DISABLED_CopyPlane) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_METAL, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	AMFPlanePtr plane2;
	amf_size origin[3] = { 0, 0, 0 };
	amf_size region[3] = { 1, 1, 0 };
	float color[4] = { 1, 1, 0, 0 };
	EXPECT_EQ(pCompute->CopyPlane(plane, origin, region, plane2, origin), AMF_OK);
	EXPECT_TRUE(plane2);
}

TEST_F(Metal_AMFCompute, CopyBufferToHost) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFBufferPtr buffer;
	context1->AllocBuffer(AMF_MEMORY_METAL, 1024, &buffer);
	void* dest = malloc(1024);
	pCompute->CopyBufferToHost(buffer, 0, 1024, dest, true);
	EXPECT_TRUE(dest);
}

TEST_F(Metal_AMFCompute, CopyBufferFromHost) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFBufferPtr buffer;
	context1->AllocBuffer(AMF_MEMORY_METAL, 1024, &buffer);
	void* dest = malloc(1024);
	pCompute->CopyBufferToHost(buffer, 0, 1024, dest, true);
	AMFBufferPtr buffer2;
	context1->AllocBuffer(AMF_MEMORY_METAL, 1024, &buffer2);
	pCompute->CopyBufferFromHost(dest, 1024, buffer2, 0, true);
	EXPECT_TRUE(buffer2);
}

TEST_F(Metal_AMFCompute, DISABLED_CopyPlaneToHost) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_METAL, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	amf_size origin[3] = { 0, 0, 0 };
	amf_size region[3] = { 1, 1, 0 };
	float color[4] = { 1, 1, 0, 0 };
	void* dest = malloc(1024);
	pCompute->CopyPlaneToHost(plane, origin, region, dest, 1024, true);
	EXPECT_TRUE(dest);
}

TEST_F(Metal_AMFCompute, DISABLED_CopyPlaneFromHost) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_METAL, AMF_SURFACE_RGBA, 2, 2, &surface);
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

TEST_F(Metal_AMFCompute, DISABLED_ConvertPlaneToPlane) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_METAL, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	AMFPlanePtr plane2;
	pCompute->ConvertPlaneToPlane(plane, &plane2, AMF_CHANNEL_ORDER_R, AMF_CHANNEL_UNSIGNED_INT32);
	EXPECT_TRUE(plane2);
}

// AMFComputeKernel

struct Metal_AMFComputeKernel : testing::Test {
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

	Metal_AMFComputeKernel() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		context1->GetMetalComputeFactory(&computeFactory);
		context1->InitMetal();
		g_AMFFactory.Init();
		startTime = initiateTestLog();
	}

	~Metal_AMFComputeKernel() {
		context1.Release();
		computeFactory.Release();
		g_AMFFactory.Terminate();
		helper.Terminate();
		terminateTestLog(startTime);
	}
};

// Causes crash, needs to be investigated
TEST_F(Metal_AMFComputeKernel, DISABLED_GetCompileWorkgroupSize) {
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
	*size = (amf_size)64;

	res = pKernel->GetCompileWorkgroupSize(size);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal_AMFComputeKernel, DISABLED_Enqueue) {
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

// AMFComponent

struct Metal_AMFComponent : testing::Test {
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

	Metal_AMFComponent() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		context1->GetMetalComputeFactory(&computeFactory);
		context1->InitMetal();
		g_AMFFactory.Init();
		startTime = initiateTestLog();
	}

	~Metal_AMFComponent() {
		context1.Release();
		computeFactory.Release();
		g_AMFFactory.Terminate();
		helper.Terminate();
		terminateTestLog(startTime);
	}
};

TEST_F(Metal_AMFComponent, DISABLED_Init) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal_AMFComponent, DISABLED_ReInit) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal_AMFComponent, DISABLED_Terminate) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal_AMFComponent, DISABLED_SubmitInput) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal_AMFComponent, DISABLED_QueryOutput) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal_AMFComponent, DISABLED_Drain) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal_AMFComponent, DISABLED_Flush) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal_AMFComponent, DISABLED_SetOutputDataAllocatorCB) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal_AMFComponent, DISABLED_GetCaps) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

// AMFCaps

struct Metal_AMFCaps : testing::Test {
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

	Metal_AMFCaps() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		context1->GetMetalComputeFactory(&computeFactory);
		context1->InitMetal();
		g_AMFFactory.Init();
		startTime = initiateTestLog();
	}

	~Metal_AMFCaps() {
		context1.Release();
		computeFactory.Release();
		g_AMFFactory.Terminate();
		helper.Terminate();
		terminateTestLog(startTime);
	}
};

TEST_F(Metal_AMFCaps, DISABLED_GetInputCaps) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal_AMFCaps, DISABLED_GetOutputCaps) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

// AMFIOCaps

struct Metal_AMFIOCaps : testing::Test {
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

	Metal_AMFIOCaps() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		context1->GetMetalComputeFactory(&computeFactory);
		context1->InitMetal();
		g_AMFFactory.Init();
		startTime = initiateTestLog();
	}

	~Metal_AMFIOCaps() {
		context1.Release();
		computeFactory.Release();
		g_AMFFactory.Terminate();
		helper.Terminate();
		terminateTestLog(startTime);
	}
};

TEST_F(Metal_AMFIOCaps, DISABLED_GetFormatAt) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal_AMFIOCaps, DISABLED_GetMemoryTypeAt) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

// AMFDataAllocator

struct Metal_AMFDataAllocatorCB : testing::Test {
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

	Metal_AMFDataAllocatorCB() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		context1->GetMetalComputeFactory(&computeFactory);
		context1->InitMetal();
		g_AMFFactory.Init();
		startTime = initiateTestLog();
	}

	~Metal_AMFDataAllocatorCB() {
		context1.Release();
		computeFactory.Release();
		g_AMFFactory.Terminate();
		helper.Terminate();
		terminateTestLog(startTime);
	}
};

TEST_F(Metal_AMFDataAllocatorCB, DISABLED_AllocBuffer) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal_AMFDataAllocatorCB, DISABLED_AllocSurface) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}
