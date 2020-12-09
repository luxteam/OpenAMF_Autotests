#include "Autotests.h"

static AMFFactoryHelper helper;
static AMFContextPtr context1;
static AMFFactory* factory;
static AMF_RESULT res;
static chrono::time_point<chrono::system_clock> startTime;

struct AMF_PropertyStorage : testing::Test {
	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_PropertyStorage");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_PropertyStorage() {
		startTime = initiateTestLog();
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	}

	~AMF_PropertyStorage() {
		context1.Release();
		terminateTestLog(startTime);
	}
};

TEST_F(AMF_PropertyStorage, AMFPropertyStorage_SetProperty) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_EQ(res, AMF_OK);
	res = context1->SetProperty(L"TestProperty", variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(AMF_PropertyStorage, AMFPropertyStorage_GetProperty) {
	AMFVariantStruct variant, res_variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_EQ(res, AMF_OK);
	res = context1->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(res, AMF_OK);
	res = context1->GetProperty(L"TestProperty", &res_variant);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
	res = context1->GetProperty(L"NotAProperty", &res_variant);
	EXPECT_EQ(res, AMF_NOT_FOUND);
}


TEST_F(AMF_PropertyStorage, AMFPropertyStorage_GetPropertyString) {
	AMFVariantStruct variant, res_variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignInt64(&variant, (amf_int64)2);
	EXPECT_EQ(res, AMF_OK);
	res = context1->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(res, AMF_OK);
	const char* res_str;
	res = context1->GetPropertyString(L"TestProperty", &res_str);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_STREQ(res_str, "2");
}

TEST_F(AMF_PropertyStorage, AMFPropertyStorage_GetPropertyWString) {
	AMFVariantStruct variant, res_variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignInt64(&variant, (amf_int64)2);
	EXPECT_EQ(res, AMF_OK);
	res = context1->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(res, AMF_OK);
	const wchar_t* res_str;
	res = context1->GetPropertyWString(L"TestProperty", &res_str);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_STREQ(res_str, L"2");
}

// TODO: Ask how to use this function properly
TEST_F(AMF_PropertyStorage, DISABLED_AMFPropertyStorage_GetPropertyAt) {
	AMFVariantStruct variant, res_variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_EQ(res, AMF_OK);
	res = context1->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(res, AMF_OK);
	res = context1->GetPropertyAt((amf_size)0, L"TestProperty", 12, &res_variant);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
}

TEST_F(AMF_PropertyStorage, AMFPropertyStorage_Clear) {
	AMFVariantStruct variant, res_variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_EQ(res, AMF_OK);
	res = context1->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(res, AMF_OK);
	res = context1->Clear();
	EXPECT_EQ(res, AMF_OK);
	res = context1->GetProperty(L"TestProperty", &res_variant);
	EXPECT_EQ(res, AMF_NOT_FOUND);
}

TEST_F(AMF_PropertyStorage, AMFPropertyStorage_AddTo) {
	AMFContextPtr context2;
	res = factory->CreateContext(&context2);
	EXPECT_EQ(res, AMF_OK);
	AMFVariantStruct variant, res_variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_EQ(res, AMF_OK);
	res = context1->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(res, AMF_OK);
	res = context1->AddTo(context2, true, true);
	EXPECT_EQ(res, AMF_OK);
	res = context2->GetProperty(L"TestProperty", &res_variant);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
}

TEST_F(AMF_PropertyStorage, AMFPropertyStorage_CopyTo) {
	AMFContextPtr context2;
	res = factory->CreateContext(&context2);
	EXPECT_EQ(res, AMF_OK);
	AMFVariantStruct variant, res_variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_EQ(res, AMF_OK);
	res = context1->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(res, AMF_OK);
	res = context1->CopyTo(context2, true);
	res = context2->GetProperty(L"TestProperty", &res_variant);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
}

// TODO: Use AMF(Component/Input/Output) to implement the following 2 cases when CreateComponent is implemented
TEST_F(AMF_PropertyStorage, DISABLED_AMFPropertyStorageEx_GetPropertyInfo) {
	AMFVariantStruct variant, res_variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_EQ(res, AMF_OK);
	//context1->SetProperty(L"TestProperty", variant);
	AMFComponent* component;
	const AMFPropertyInfo** info = NULL;
	res = factory->CreateComponent(context1, L"component", &component);
	EXPECT_EQ(res, AMF_OK);
	res = component->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(res, AMF_OK);
	res = component->GetPropertyInfo(L"TestProperty", info);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(AMF_PropertyStorage, DISABLED_AMFPropertyStorage_ValidateProperty) {
	EXPECT_EQ(res, AMF_OK);
}
