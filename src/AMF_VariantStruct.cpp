#include "autotests.h"

static AMFFactoryHelper helper;
static AMFContextPtr context1;
static AMFFactory* factory;
static AMF_RESULT res;
static chrono::time_point<chrono::system_clock> startTime;

struct AMF_VariantStruct : testing::Test {
	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_VariantStruct");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_VariantStruct() {
		startTime = initiateTestLog();
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	}

	~AMF_VariantStruct() {
		context1.Release();
		terminateTestLog(startTime);
	}
};

TEST_F(AMF_VariantStruct, AMFVariantStruct_AMFVariantInit) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(AMF_VariantStruct, AMFVariantStruct_AMFVariantClear) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignString(&variant, "ТеStパーティーへ行かないか");
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantClear(&variant);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(AMF_VariantStruct, AMFVariantStruct_AMFVariantAssignBool) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignBool(&variant, true);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetBool(&variant), true);
}

TEST_F(AMF_VariantStruct, AMFVariantStruct_AMFVariantAssignInt64) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignInt64(&variant, (amf_int64)4);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetInt64(&variant), (amf_int64)4);
}

TEST_F(AMF_VariantStruct, AMFVariantStruct_AMFVariantAssignDouble) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetDouble(&variant), (amf_double) 2.74);
}

TEST_F(AMF_VariantStruct, AMFVariantStruct_AMFVariantAssignString) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignString(&variant, "ТеStパーティーへ行かないか");
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(*AMFVariantGetString(&variant), *"ТеStパーティーへ行かないか");
}

TEST_F(AMF_VariantStruct, AMFVariantStruct_AMFVariantAssignWString) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignWString(&variant, L"ТеStパーティーへ行かないか");
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(*AMFVariantGetWString(&variant), *L"ТеStパーティーへ行かないか");
}

TEST_F(AMF_VariantStruct, AMFVariantStruct_AMFVariantAssignInterface) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	AMFInterface* inter = NULL;
	res = AMFVariantAssignInterface(&variant, inter);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(AMF_VariantStruct, AMFVariantStruct_AMFVariantAssignRect) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	AMFRect* rect = new AMFRect;
	res = AMFVariantAssignRect(&variant, rect);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetRect(&variant), *rect);
}

TEST_F(AMF_VariantStruct, AMFVariantStruct_AMFVariantAssignSize) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	AMFSize* size = new AMFSize;
	res = AMFVariantAssignSize(&variant, size);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetSize(&variant), *size);
}

TEST_F(AMF_VariantStruct, AMFVariantStruct_AMFVariantAssignPoint) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	AMFPoint* point = new AMFPoint;
	res = AMFVariantAssignPoint(&variant, point);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetPoint(&variant), *point);
}


TEST_F(AMF_VariantStruct, AMFVariantStruct_AMFVariantAssignRatio) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	AMFRatio* ratio = new AMFRatio;
	res = AMFVariantAssignRatio(&variant, ratio);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetRatio(&variant), *ratio);
}

TEST_F(AMF_VariantStruct, AMFVariantStruct_AMFVariantAssignColor) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	AMFColor* color = new AMFColor;
	color->r = 1; color->g = 0.5; color->b = 0; color->a = 0;
	res = AMFVariantAssignColor(&variant, color);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetColor(&variant), *color);
}

TEST_F(AMF_VariantStruct, AMFVariantStruct_AMFVariantCompare) {
	AMFVariantStruct variant1, variant2, variant3;
	res = AMFVariantInit(&variant1);
	EXPECT_EQ(res, AMF_OK); 
	res = AMFVariantInit(&variant2);
	EXPECT_EQ(res, AMF_OK);
	AMFColor* color = new AMFColor;
	AMFRatio* ratio = new AMFRatio;
	res = AMFVariantAssignColor(&variant1, color);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignColor(&variant2, color);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignRatio(&variant3, ratio);
	EXPECT_EQ(res, AMF_OK);
	amf_bool equal;
	res = AMFVariantCompare(&variant1, &variant2, &equal);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_TRUE(equal);
	res = AMFVariantCompare(&variant1, &variant3, &equal);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_FALSE(equal);
}

TEST_F(AMF_VariantStruct, AMFVariantStruct_AMFVariantCopy) {
	AMFVariantStruct variant1, variant2;
	res = AMFVariantInit(&variant1);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantInit(&variant2);
	EXPECT_EQ(res, AMF_OK);
	AMFColor* color = new AMFColor;
	color->r = 1; color->g = 0.5; color->b = 0; color->a = 0;
	res = AMFVariantAssignColor(&variant2, color);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantCopy(&variant1, &variant2);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(AMF_VariantStruct, AMFVariantStruct_AMFVariantChangeType) {
	AMFVariantStruct variant1, variant2;
	res = AMFVariantInit(&variant1);
	EXPECT_EQ(res, AMF_OK);
	amf_int64 in = 1;
	res = AMFVariantAssignInt64(&variant1, in);
	EXPECT_EQ(res, AMF_OK);
	amf_double fl = 2.2;
	res = AMFVariantAssignDouble(&variant2, fl);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantChangeType(&variant2, &variant1, AMF_VARIANT_DOUBLE);
	EXPECT_EQ(AMFVariantGetType(&variant1), AMF_VARIANT_DOUBLE);
	EXPECT_EQ(AMFVariantGetType(&variant2), AMF_VARIANT_DOUBLE);
}