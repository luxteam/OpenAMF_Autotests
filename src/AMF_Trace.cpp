#include "autotests.h"

static AMFFactoryHelper helper;
static AMFContextPtr context1;
static AMFFactory* factory;
static AMF_RESULT res;
static chrono::time_point<chrono::system_clock> startTime;

struct AMF_Trace : testing::Test {
	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_Trace");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Trace() {
		startTime = initiateTestLog();
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	}

	~AMF_Trace() {
		context1.Release();
		terminateTestLog(startTime);
	}
};

TEST_F(AMF_Trace, AMFTrace_SetPath) {
	AMFTrace* trace;
	factory->GetTrace(&trace);
	res = trace->SetPath(L"test");
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(AMF_Trace, DISABLED_AMFTrace_GetPath) {
	AMFTrace* trace;
	factory->GetTrace(&trace);
	res = trace->SetPath(L"test");
	amf_size* size;
	*size = (amf_size)30;
	wchar_t* str = new wchar_t[1000];
	res = trace->GetPath(str, size);
	EXPECT_EQ(str, L"test");
}