#include "autotests.h"

struct Geometry : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFComputeFactoryPtr oclComputeFactory;
	AMFFactory* factory;
	int deviceCount;
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("Geometry");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	Geometry() {
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

	~Geometry() {
		context1.Release();
		oclComputeFactory.Release();
		g_AMFFactory.Terminate();
		helper.Terminate();
		terminateTestLog(startTime);
	}
};

TEST_F(Geometry, geometry_test) {
	AMFRect rect{0, 1, 1, 0};
	EXPECT_EQ(rect.Height(), -rect.Width());
	EXPECT_EQ(rect.Height(), -1);
}
