#include "Autotests.h"

static AMFFactoryHelper helper;
static AMFContextPtr context1;
static AMFFactory* factory;
static AMF_RESULT res;
static chrono::time_point<chrono::system_clock> startTime;

struct AMF_Factory : testing::Test {
	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_Factory");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Factory() {
		startTime = initiateTestLog();
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	}

	~AMF_Factory() {
		context1.Release();
		terminateTestLog(startTime);
	}
};

TEST_F(AMF_Factory, AMFFactory_CreateContext) {
	res = factory->CreateContext(&context1);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_TRUE(context1);
}

TEST_F(AMF_Factory, DISABLED_AMFFactory_CreateComponent) {
	AMFComponentPtr component;
	res = factory->CreateComponent(context1, 0, &component);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_TRUE(component);
}

TEST_F(AMF_Factory, AMFFactory_SetCacheFolder) {
	res = factory->SetCacheFolder(L"test");
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(factory->GetCacheFolder(), L"test");
}

TEST_F(AMF_Factory, AMFFactory_GetDebug) {
	AMFDebug* debug;
	res = factory->GetDebug(&debug);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_TRUE(debug);
}

TEST_F(AMF_Factory, AMFFactory_GetTrace) {
	AMFTrace* trace;
	res = factory->GetTrace(&trace);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_TRUE(trace);
}

TEST_F(AMF_Factory, AMFFactory_GetPrograms) {
	AMFPrograms* programs;
	res = factory->GetPrograms(&programs);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_TRUE(programs);
}
