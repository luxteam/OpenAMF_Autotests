#include "Autotests.h"

struct Implementation : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFFactory* factory;
	AMF_RESULT res;
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("Implementation");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	Implementation() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		startTime = initiateTestLog();
	}

	~Implementation() {
		terminateTestLog(startTime);
	}
};

TEST_F(Implementation, AMFContext_InitOpenCL) {
	AMFComputeFactoryPtr fact;
	res = context1->InitOpenCL();
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFContext_InitMetal) {
	AMFComputeFactoryPtr fact;
	res = context1->InitMetal();
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFContext_InitDX11_GetDX11Device) {
	res = context1->InitDX11(context1->GetDX11Device());
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFContext_InitDX9_GetDX9Device) {
	res = context1->InitDX9(context1->GetDX9Device());
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFContext_InitOpenGL_GetOpenGLContext) {
	res = context1->InitOpenGL(context1->GetOpenGLContext(), NULL, NULL);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFContext_InitXV_GetXVDevice) {
	res = context1->InitXV(context1->GetXVDevice());
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFContext_InitGralloc_GetGrallocDevice) {
	res = context1->InitGralloc(context1->GetGrallocDevice());
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFContext_LockOpenCL_UnlockOpenCL) {
	res = context1->LockOpenCL();
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	res = context1->UnlockOpenCL();
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}
