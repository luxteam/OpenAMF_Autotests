#include "autotests.h"

struct Smoke : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFComputeFactoryPtr oclComputeFactory;
	AMFFactory* factory;
	int deviceCount;
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		//_CrtSetAllocHook(allocationHook);
		initiateTestSuiteLog("Smoke");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	Smoke() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		context1->GetOpenCLComputeFactory(&oclComputeFactory);
		context1->InitOpenCL();
		deviceCount = oclComputeFactory->GetDeviceCount();
		g_AMFFactory.Init();
		g_AMFFactory.GetDebug()->AssertsEnable(true);
		g_AMFFactory.GetTrace()->SetWriterLevel(AMF_TRACE_WRITER_FILE, AMF_TRACE_TRACE);
		g_AMFFactory.GetTrace()->SetGlobalLevel(AMF_TRACE_TRACE);
		g_AMFFactory.GetTrace()->SetWriterLevel(AMF_TRACE_WRITER_CONSOLE, AMF_TRACE_TRACE);
		g_AMFFactory.GetTrace()->SetWriterLevelForScope(AMF_TRACE_WRITER_CONSOLE, L"scope2", AMF_TRACE_TRACE);
		g_AMFFactory.GetTrace()->SetWriterLevelForScope(AMF_TRACE_WRITER_CONSOLE, L"scope2", AMF_TRACE_ERROR);
		startTime = initiateTestLog();
	}

	~Smoke() {
		context1.Release();
		oclComputeFactory.Release();
		g_AMFFactory.Terminate();
		helper.Terminate();
		terminateTestLog(startTime);
	}
};

TEST_F(Smoke, set_cache_folder) {
	g_AMFFactory.GetFactory()->SetCacheFolder(L"./cache");
	EXPECT_STREQ(g_AMFFactory.GetFactory()->GetCacheFolder(), L"./cache");
}

TEST_F(Smoke, release_null_check) {
	context1.Release();
	EXPECT_EQ(context1, (amf::AMFContextPtr)NULL);
}

TEST_F(Smoke, DISABLED_traceW_error) {
	g_AMFFactory.GetTrace()->SetPath(L"traceW.log");
	g_AMFFactory.GetTrace()->TraceW(L"path", 387, AMF_TRACE_ERROR, L"scope", 4, L"Error message");
	fstream fd;
	fd.open("traceW.log", ios::in);
	string log;
	getline(fd, log);
	EXPECT_TRUE(has_suffix(log, (string)"Error message"));
}

TEST_F(Smoke, rect_test) {
	AMFRect rect{ 0, 1, 1, 0 };
	EXPECT_EQ(rect.Height(), -rect.Width());
	EXPECT_EQ(rect.Height(), -1);
}

TEST_F(Smoke, DISABLED_get_compute) {
	AMFCompute* compute;
	context1->GetCompute(AMF_MEMORY_OPENCL, &compute);
	EXPECT_TRUE(compute);
}

TEST_F(Smoke, computeFactory_getDeviceCount) {
	EXPECT_EQ(oclComputeFactory->GetDeviceCount(), 1);
}

TEST_F(Smoke, deviceCompute_getNativePlatform) {
	AMFComputeDevice* device;
	EXPECT_EQ(oclComputeFactory->GetDeviceAt(0, &device), AMF_OK);
	EXPECT_TRUE(device->GetNativePlatform());
}

TEST_F(Smoke, deviceCompute_getNativeDeviceID) {
	AMFComputeDevice* device;
	EXPECT_EQ(oclComputeFactory->GetDeviceAt(0, &device), AMF_OK);
	EXPECT_TRUE(device->GetNativeDeviceID());
}

TEST_F(Smoke, deviceCompute_getNativeContext) {
	AMFComputeDevice* device;
	EXPECT_EQ(oclComputeFactory->GetDeviceAt(0, &device), AMF_OK);
	EXPECT_TRUE(device->GetNativeContext());
}

TEST_F(Smoke, compute_getMemoryTypes) {
	AMFComputeDevice* device;
	oclComputeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	EXPECT_EQ(device->CreateCompute(nullptr, &pCompute), AMF_OK);
	EXPECT_TRUE(pCompute->GetMemoryType());
}

TEST_F(Smoke, compute_getNativeContext) {
	AMFComputeDevice* device;
	oclComputeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	EXPECT_EQ(device->CreateCompute(nullptr, &pCompute), AMF_OK);
	EXPECT_TRUE(pCompute->GetNativeContext());
}

TEST_F(Smoke, compute_getNativeDeviceID) {
	AMFComputeDevice* device;
	oclComputeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	EXPECT_EQ(device->CreateCompute(nullptr, &pCompute), AMF_OK);
	EXPECT_TRUE(pCompute->GetNativeDeviceID());
}

TEST_F(Smoke, compute_getNativeCommandQueue) {
	AMFComputeDevice* device;
	oclComputeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	EXPECT_EQ(device->CreateCompute(nullptr, &pCompute), AMF_OK);
	EXPECT_TRUE(pCompute->GetNativeCommandQueue());
}

// make variations of this test
TEST_F(Smoke, DISABLED_compute_copyBufferToHost_blocking) {
	AMFComputeDevice* device;
	oclComputeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFBufferPtr buffer;
	context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024, &buffer);
	void* dest = malloc(1024);
	pCompute->CopyBufferToHost(buffer, 0, 1024, dest, true);
	EXPECT_TRUE(dest);
}
