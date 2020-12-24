#include "Autotests.h"

static SharedVariables variables;

struct AMF_PropertyStorage : testing::Test {
	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_PropertyStorage");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_PropertyStorage() {
		variables.startTime = initiateTestLog();
		variables.helper.Init();
		variables.factory = variables.helper.GetFactory();
		variables.factory->CreateContext(&variables.context1);
		variables.context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	}

	~AMF_PropertyStorage() {
		variables.context1.Release();
		terminateTestLog(variables.startTime);
	}
};

TEST_F(AMF_PropertyStorage, AMFPropertyStorage_SetProperty) {
	AMFVariantStruct variant;
	variables.res = AMFVariantInit(&variant);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->SetProperty(L"TestProperty", variant);
	EXPECT_NE(variables.res, AMF_NOT_IMPLEMENTED);
}

TEST_F(AMF_PropertyStorage, AMFPropertyStorage_GetProperty) {
	AMFVariantStruct variant, res_variant;
	variables.res = AMFVariantInit(&variant);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->GetProperty(L"TestProperty", &res_variant);
	EXPECT_EQ(variables.res, AMF_OK);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
	variables.res = variables.context1->GetProperty(L"NotAProperty", &res_variant);
	EXPECT_EQ(variables.res, AMF_NOT_FOUND);
}


TEST_F(AMF_PropertyStorage, AMFPropertyStorage_GetPropertyString) {
	AMFVariantStruct variant, res_variant;
	variables.res = AMFVariantInit(&variant);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = AMFVariantAssignInt64(&variant, (amf_int64)2);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(variables.res, AMF_OK);
	const char* res_str;
	variables.res = variables.context1->GetPropertyString(L"TestProperty", &res_str);
	EXPECT_EQ(variables.res, AMF_OK);
	EXPECT_STREQ(res_str, "2");
}

TEST_F(AMF_PropertyStorage, AMFPropertyStorage_GetPropertyWString) {
	AMFVariantStruct variant, res_variant;
	variables.res = AMFVariantInit(&variant);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = AMFVariantAssignInt64(&variant, (amf_int64)2);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(variables.res, AMF_OK);
	const wchar_t* res_str;
	variables.res = variables.context1->GetPropertyWString(L"TestProperty", &res_str);
	EXPECT_EQ(variables.res, AMF_OK);
	EXPECT_STREQ(res_str, L"2");
}

// TODO: Ask how to use this function properly
TEST_F(AMF_PropertyStorage, DISABLED_AMFPropertyStorage_GetPropertyAt) {
	AMFVariantStruct variant, res_variant;
	variables.res = AMFVariantInit(&variant);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->GetPropertyAt((amf_size)0, L"TestProperty", 12, &res_variant);
	EXPECT_EQ(variables.res, AMF_OK);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
}

TEST_F(AMF_PropertyStorage, AMFPropertyStorage_Clear) {
	AMFVariantStruct variant, res_variant;
	variables.res = AMFVariantInit(&variant);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->Clear();
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->GetProperty(L"TestProperty", &res_variant);
	EXPECT_EQ(variables.res, AMF_NOT_FOUND);
}

TEST_F(AMF_PropertyStorage, AMFPropertyStorage_AddTo) {
	AMFContextPtr context2;
	variables.res = variables.factory->CreateContext(&context2);
	EXPECT_EQ(variables.res, AMF_OK);
	AMFVariantStruct variant, res_variant;
	variables.res = AMFVariantInit(&variant);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->AddTo(context2, true, true);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = context2->GetProperty(L"TestProperty", &res_variant);
	EXPECT_EQ(variables.res, AMF_OK);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
}

TEST_F(AMF_PropertyStorage, AMFPropertyStorage_CopyTo) {
	AMFContextPtr context2;
	variables.res = variables.factory->CreateContext(&context2);
	EXPECT_EQ(variables.res, AMF_OK);
	AMFVariantStruct variant, res_variant;
	variables.res = AMFVariantInit(&variant);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->CopyTo(context2, true);
	variables.res = context2->GetProperty(L"TestProperty", &res_variant);
	EXPECT_EQ(variables.res, AMF_OK);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
}

// TODO: Use AMF(Component/Input/Output) to implement the following 2 cases when CreateComponent is implemented
TEST_F(AMF_PropertyStorage, DISABLED_AMFPropertyStorageEx_GetPropertyInfo) {
	AMFVariantStruct variant, res_variant;
	variables.res = AMFVariantInit(&variant);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_EQ(variables.res, AMF_OK);
	//variables.context1->SetProperty(L"TestProperty", variant);
	AMFComponent* component;
	const AMFPropertyInfo** info = NULL;
	variables.res = variables.factory->CreateComponent(variables.context1, L"component", &component);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = component->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(variables.res, AMF_OK);
	variables.res = component->GetPropertyInfo(L"TestProperty", info);
	EXPECT_EQ(variables.res, AMF_OK);
}

TEST_F(AMF_PropertyStorage, DISABLED_AMFPropertyStorage_ValidateProperty) {
	EXPECT_EQ(variables.res, AMF_OK);
}
