#include "Autotests.h"

static SharedVariables variables;

struct AMF_Factory : testing::Test {
	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_Factory");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	void initResources() {
		variables.res = variables.helper.Init();
		ASSERT_EQ(variables.res, AMF_OK);
		variables.factory = variables.helper.GetFactory();
		variables.res = variables.factory->CreateContext(&variables.context1);
		ASSERT_EQ(variables.res, AMF_OK);
		variables.res = variables.context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		ASSERT_EQ(variables.res, AMF_OK);
	}

	AMF_Factory() {
		variables.startTime = initiateTestLog();
		initResources();
	}

	~AMF_Factory() {
		variables.context1.Release();
		terminateTestLog(variables.startTime);
	}
};

TEST_F(AMF_Factory, AMFFactory_CreateContext) {
	variables.res = variables.factory->CreateContext(&variables.context1);
	EXPECT_NE(variables.res, AMF_NOT_IMPLEMENTED);
	EXPECT_TRUE(variables.context1);
}

TEST_F(AMF_Factory, DISABLED_AMFFactory_CreateComponent) {
	AMFComponentPtr component;
	variables.res = variables.factory->CreateComponent(variables.context1, 0, &component);
	EXPECT_NE(variables.res, AMF_NOT_IMPLEMENTED);
	EXPECT_TRUE(component);
}

TEST_F(AMF_Factory, AMFFactory_SetCacheFolder) {
	variables.res = variables.factory->SetCacheFolder(L"test");
	EXPECT_NE(variables.res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(variables.factory->GetCacheFolder(), L"test");
}

TEST_F(AMF_Factory, AMFFactory_GetDebug) {
	AMFDebug* debug;
	variables.res = variables.factory->GetDebug(&debug);
	EXPECT_EQ(variables.res, AMF_OK);
	EXPECT_TRUE(debug);
}

TEST_F(AMF_Factory, AMFFactory_GetTrace) {
	AMFTrace* trace;
	variables.res = variables.factory->GetTrace(&trace);
	EXPECT_EQ(variables.res, AMF_OK);
	EXPECT_TRUE(trace);
}

TEST_F(AMF_Factory, AMFFactory_GetPrograms) {
	AMFPrograms* programs;
	variables.res = variables.factory->GetPrograms(&programs);
	EXPECT_EQ(variables.res, AMF_OK);
	EXPECT_TRUE(programs);
}
