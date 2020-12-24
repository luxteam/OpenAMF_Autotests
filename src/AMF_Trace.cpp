#include "Autotests.h"

static SharedVariables variables;

struct AMF_Trace : testing::Test {
	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_Trace");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Trace() {
		variables.startTime = initiateTestLog();
		variables.helper.Init();
		variables.factory = variables.helper.GetFactory();
		variables.factory->CreateContext(&variables.context1);
		variables.context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	}

	~AMF_Trace() {
		variables.context1.Release();
		terminateTestLog(variables.startTime);
	}
};

TEST_F(AMF_Trace, AMFTrace_SetPath) {
	AMFTrace* trace;
	variables.factory->GetTrace(&trace);
	variables.res = trace->SetPath(L"test");
	EXPECT_EQ(variables.res, AMF_OK);
}

TEST_F(AMF_Trace, DISABLED_AMFTrace_GetPath) {
	AMFTrace* trace;
	variables.res = variables.factory->GetTrace(&trace);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = trace->SetPath(L"test");
	EXPECT_EQ(variables.res, AMF_OK);
	amf_size* size;
	*size = (amf_size)30;
	wchar_t* str = new wchar_t[1000];
	variables.res = trace->GetPath(str, size);
	EXPECT_EQ(variables.res, AMF_OK);
	EXPECT_EQ(str, L"test");
}
