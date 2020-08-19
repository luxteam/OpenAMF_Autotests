#include "autotests.h"

struct Implementation : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFComputeFactoryPtr oclComputeFactory;
	AMFFactory* factory;
	int deviceCount;
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
		g_AMFFactory.Init();
		startTime = initiateTestLog();
	}

	~Implementation() {
		context1.Release();
		oclComputeFactory.Release();
		g_AMFFactory.Terminate();
		helper.Terminate();
		terminateTestLog(startTime);
	}
};

TEST_F(Implementation, openCL_compute_factory_impl) {
	AMFComputeFactoryPtr fact;
	res = context1->GetOpenCLComputeFactory(&fact);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, DISABLED_DX11_impl) {
	res = context1->InitDX11(context1->GetDX11Device());
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, DISABLED_DX9_impl) {
	res = context1->InitDX9(context1->GetDX9Device());
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, DISABLED_openGL_impl) {
	res = context1->InitOpenGL(context1->GetOpenGLContext(), NULL, NULL);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, DISABLED_xv_impl) {
	res = context1->InitXV(context1->GetXVDevice());
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, DISABLED_grall_impl) {
	res = context1->InitGralloc(context1->GetGrallocDevice());
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, DISABLED_opencl_lock_unlock_impl) {
	res = context1->LockOpenCL();
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	res = context1->UnlockOpenCL();
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

////////////////////////////////////////////////////////////////////////////////////

TEST_F(Implementation, AMFFactory_CreateContext) {
	res = factory->CreateContext(&context1);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFFactory_CreateComponent) {
	AMFComponentPtr component;
	res = factory->CreateComponent(context1, 0, &component);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFFactory_SetCacheFolder) {
	res = factory->SetCacheFolder(L"test");
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFFactory_GetDebug) {
	AMFDebug* debug;
	res = factory->GetDebug(&debug);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFFactory_GetTrace) {
	AMFTrace* trace;
	res = factory->GetTrace(&trace);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFFactory_GetPrograms) {
	AMFPrograms* programs;
	res = factory->GetPrograms(&programs);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}


TEST_F(Implementation, AMFTrace_SetPath) {
	AMFTrace* trace;
	factory->GetTrace(&trace);
	res = trace->SetPath(L"test");
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFTrace_GetPath) {
	AMFTrace* trace;
	factory->GetTrace(&trace);
	amf_size* size;
	res = trace->GetPath(new wchar_t[1000], size);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantInit) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantClear) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantClear(&variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantAssignBool) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantAssignBool(&variant, true);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetBool(&variant), true);
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantAssignInt64) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantAssignInt64(&variant, (amf_int64) 4);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetInt64(&variant), (amf_int64) 4);
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantAssignDouble) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetDouble(&variant), (amf_double) 2.74);
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantAssignString) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantAssignString(&variant, "ТеStパーティーへ行かないか");
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(*AMFVariantGetString(&variant), *"ТеStパーティーへ行かないか");
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantAssignWString) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantAssignWString(&variant, L"ТеStパーティーへ行かないか");
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(*AMFVariantGetWString(&variant), *L"ТеStパーティーへ行かないか");
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantAssignInterface) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFInterface* inter;
	res = AMFVariantAssignInterface(&variant, inter);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantAssignRect) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFRect* rect;
	res = AMFVariantAssignRect(&variant, rect);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetRect(&variant), *rect);
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantAssignSize) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFSize* size;
	res = AMFVariantAssignSize(&variant, size);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetSize(&variant), *size);
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantAssignPoint) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFPoint* point;
	res = AMFVariantAssignPoint(&variant, point);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetPoint(&variant), *point);
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantAssignRate) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFRate* rate;
	res = AMFVariantAssignRate(&variant, rate);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetRate(&variant), *rate);
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantAssignRatio) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFRatio* ratio;
	res = AMFVariantAssignRatio(&variant, ratio);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetRatio(&variant), *ratio);
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantAssignColor) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFColor* color;
	res = AMFVariantAssignColor(&variant, color);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetColor(&variant), *color);
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantCompare) {
	AMFVariantStruct variant1, variant2, variant3;
	AMFVariantInit(&variant1); AMFVariantInit(&variant2);
	AMFColor* color;
	AMFRatio* ratio;
	AMFVariantAssignColor(&variant1, color);
	AMFVariantAssignColor(&variant2, color);
	AMFVariantAssignRatio(&variant3, ratio);
	amf_bool equal;
	res = AMFVariantCompare(&variant1, &variant2, &equal);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_TRUE(equal);
	res = AMFVariantCompare(&variant1, &variant3, &equal);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_FALSE(equal);
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantCopy) {
	AMFVariantStruct variant1, variant2;
	AMFVariantInit(&variant1); AMFVariantInit(&variant2);
	AMFColor* color;
	AMFVariantAssignColor(&variant2, color);
	res = AMFVariantCopy(&variant1, &variant2);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFVariantStruct_AMFVariantChangeType) {
	AMFVariantStruct variant1, variant2;
	AMFVariantInit(&variant1);
	amf_int64 in = 1;
	AMFVariantAssignInt64(&variant1, in);
	amf_double fl = 2.2;
	AMFVariantAssignDouble(&variant2, fl);
	res = AMFVariantChangeType(&variant2, &variant1, AMF_VARIANT_DOUBLE);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFPropertyStorage_SetProperty) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	res = context1->SetProperty(L"TestProperty", variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, AMFPropertyStorage_GetProperty) {
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	context1->SetProperty(L"TestProperty", variant);
	res = context1->GetProperty(L"TestProperty", &res_variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
	res = context1->GetProperty(L"NotAProperty", &res_variant);
	EXPECT_EQ(res, AMF_NOT_FOUND);
}


TEST_F(Implementation, DISABLED_AMFPropertyStorage_GetPropertyString) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, DISABLED_AMFPropertyStorage_GetPropertyWString) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, DISABLED_AMFPropertyStorage_GetPropertyAt) {
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	context1->SetProperty(L"TestProperty", variant);
	res = context1->GetPropertyAt(0, L"TestProperty", 12, &res_variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
}

TEST_F(Implementation, AMFPropertyStorage_Clear) {
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	context1->SetProperty(L"TestProperty", variant);
	context1->Clear();
	res = context1->GetProperty(L"TestProperty", &res_variant);
	EXPECT_EQ(res, AMF_NOT_FOUND);
}

TEST_F(Implementation, AMFPropertyStorage_AddTo) {
	AMFContextPtr context2;
	factory->CreateContext(&context2);
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	context1->SetProperty(L"TestProperty", variant);
	res = context1->AddTo(context2, true, true);
	context2->GetProperty(L"TestProperty", &res_variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
}

TEST_F(Implementation, AMFPropertyStorage_CopyTo) {
	AMFContextPtr context2;
	factory->CreateContext(&context2);
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	context1->SetProperty(L"TestProperty", variant);
	res = context1->CopyTo(context2, true);
	context2->GetProperty(L"TestProperty", &res_variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
}

// TODO: Use AMF(Component/Input/Output) to implement the following 2 cases
TEST_F(Implementation, DISABLED_AMFPropertyStorageEx_GetPropertyInfo) {
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	context1->SetProperty(L"TestProperty", variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Implementation, DISABLED_AMFPropertyStorage_ValidateProperty) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}