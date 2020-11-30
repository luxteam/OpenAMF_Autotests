#include "autotests.h"
#include "../../common/Thread.h"

struct General : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFFactory* factory;
	AMF_RESULT res;
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("General");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	General() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		startTime = initiateTestLog();
	}

	~General() {
		context1.Release();
		terminateTestLog(startTime);
	}
};

TEST_F(General, AMFFactory_CreateContext) {
	res = factory->CreateContext(&context1);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_TRUE(context1);
}

TEST_F(General, DISABLED_AMFFactory_CreateComponent) {
	AMFComponentPtr component;
	res = factory->CreateComponent(context1, 0, &component);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_TRUE(component);
}

TEST_F(General, AMFFactory_SetCacheFolder) {
	res = factory->SetCacheFolder(L"test");
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(factory->GetCacheFolder(), L"test");
}

TEST_F(General, AMFFactory_GetDebug) {
	AMFDebug* debug;
	res = factory->GetDebug(&debug);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_TRUE(debug);
}

TEST_F(General, AMFFactory_GetTrace) {
	AMFTrace* trace;
	res = factory->GetTrace(&trace);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_TRUE(trace);
}

TEST_F(General, AMFFactory_GetPrograms) {
	AMFPrograms* programs;
	res = factory->GetPrograms(&programs);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_TRUE(programs);
}


TEST_F(General, AMFTrace_SetPath) {
	AMFTrace* trace;
	factory->GetTrace(&trace);
	res = trace->SetPath(L"test");
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(General, DISABLED_AMFTrace_GetPath) {
	AMFTrace* trace;
	factory->GetTrace(&trace);
	res = trace->SetPath(L"test");
	amf_size* size;
	*size = (amf_size)30;
	wchar_t* str = new wchar_t[1000];
	res = trace->GetPath(str, size);
	EXPECT_EQ(str, L"test");
}

TEST_F(General, AMFVariantStruct_AMFVariantInit) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(General, AMFVariantStruct_AMFVariantClear) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFVariantAssignString(&variant, "ТеStパーティーへ行かないか");
	res = AMFVariantClear(&variant);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(General, AMFVariantStruct_AMFVariantAssignBool) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantAssignBool(&variant, true);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetBool(&variant), true);
}

TEST_F(General, AMFVariantStruct_AMFVariantAssignInt64) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantAssignInt64(&variant, (amf_int64) 4);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetInt64(&variant), (amf_int64) 4);
}

TEST_F(General, AMFVariantStruct_AMFVariantAssignDouble) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetDouble(&variant), (amf_double) 2.74);
}

TEST_F(General, AMFVariantStruct_AMFVariantAssignString) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantAssignString(&variant, "ТеStパーティーへ行かないか");
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(*AMFVariantGetString(&variant), *"ТеStパーティーへ行かないか");
}

TEST_F(General, AMFVariantStruct_AMFVariantAssignWString) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantAssignWString(&variant, L"ТеStパーティーへ行かないか");
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(*AMFVariantGetWString(&variant), *L"ТеStパーティーへ行かないか");
}

TEST_F(General, AMFVariantStruct_AMFVariantAssignInterface) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFInterface* inter = NULL;
	res = AMFVariantAssignInterface(&variant, inter);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(General, AMFVariantStruct_AMFVariantAssignRect) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFRect* rect = new AMFRect;
	res = AMFVariantAssignRect(&variant, rect);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetRect(&variant), *rect);
}

TEST_F(General, AMFVariantStruct_AMFVariantAssignSize) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFSize* size = new AMFSize;
	res = AMFVariantAssignSize(&variant, size);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetSize(&variant), *size);
}

TEST_F(General, AMFVariantStruct_AMFVariantAssignPoint) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFPoint* point = new AMFPoint;
	res = AMFVariantAssignPoint(&variant, point);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetPoint(&variant), *point);
}


TEST_F(General, AMFVariantStruct_AMFVariantAssignRatio) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFRatio* ratio = new AMFRatio;
	res = AMFVariantAssignRatio(&variant, ratio);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetRatio(&variant), *ratio);
}

TEST_F(General, AMFVariantStruct_AMFVariantAssignColor) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFColor* color = new AMFColor;
	color->r = 1; color->g = 0.5; color->b = 0; color->a = 0;
	res = AMFVariantAssignColor(&variant, color);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetColor(&variant), *color);
}

TEST_F(General, AMFVariantStruct_AMFVariantCompare) {
	AMFVariantStruct variant1, variant2, variant3;
	AMFVariantInit(&variant1); AMFVariantInit(&variant2);
	AMFColor* color = new AMFColor;
	AMFRatio* ratio = new AMFRatio;
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

TEST_F(General, AMFVariantStruct_AMFVariantCopy) {
	AMFVariantStruct variant1, variant2;
	AMFVariantInit(&variant1); AMFVariantInit(&variant2);
	AMFColor* color = new AMFColor;
	color->r = 1; color->g = 0.5; color->b = 0; color->a = 0;
	AMFVariantAssignColor(&variant2, color);
	res = AMFVariantCopy(&variant1, &variant2);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(General, AMFVariantStruct_AMFVariantChangeType) {
	AMFVariantStruct variant1, variant2;
	AMFVariantInit(&variant1);
	amf_int64 in = 1;
	AMFVariantAssignInt64(&variant1, in);
	amf_double fl = 2.2;
	AMFVariantAssignDouble(&variant2, fl);
	res = AMFVariantChangeType(&variant2, &variant1, AMF_VARIANT_DOUBLE);
	EXPECT_EQ(AMFVariantGetType(&variant1), AMF_VARIANT_DOUBLE);
	EXPECT_EQ(AMFVariantGetType(&variant2), AMF_VARIANT_DOUBLE);
}

TEST_F(General, AMFPropertyStorage_SetProperty) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	res = context1->SetProperty(L"TestProperty", variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(General, AMFPropertyStorage_GetProperty) {
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


TEST_F(General, AMFPropertyStorage_GetPropertyString) {
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignInt64(&variant, (amf_int64) 2);
	context1->SetProperty(L"TestProperty", variant);
	const char* res_str;
	res = context1->GetPropertyString(L"TestProperty", &res_str);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_STREQ(res_str, "2");
}

TEST_F(General, AMFPropertyStorage_GetPropertyWString) {
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignInt64(&variant, (amf_int64)2);
	context1->SetProperty(L"TestProperty", variant);
	const wchar_t* res_str;
	res = context1->GetPropertyWString(L"TestProperty", &res_str);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_STREQ(res_str, L"2");
}

// TODO: Ask how to use this function properly
TEST_F(General, DISABLED_AMFPropertyStorage_GetPropertyAt) {
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	context1->SetProperty(L"TestProperty", variant);
	res = context1->GetPropertyAt((amf_size) 0, L"TestProperty", 12, &res_variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
}

TEST_F(General, AMFPropertyStorage_Clear) {
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	context1->SetProperty(L"TestProperty", variant);
	context1->Clear();
	res = context1->GetProperty(L"TestProperty", &res_variant);
	EXPECT_EQ(res, AMF_NOT_FOUND);
}

TEST_F(General, AMFPropertyStorage_AddTo) {
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

TEST_F(General, AMFPropertyStorage_CopyTo) {
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

// TODO: Use AMF(Component/Input/Output) to implement the following 2 cases when CreateComponent is implemented
TEST_F(General, DISABLED_AMFPropertyStorageEx_GetPropertyInfo) {
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	//context1->SetProperty(L"TestProperty", variant);
	AMFComponent* component;
	const AMFPropertyInfo** info = NULL;
	factory->CreateComponent(context1, L"component", &component);
	component->SetProperty(L"TestProperty", variant);
	res = component->GetPropertyInfo(L"TestProperty", info);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(General, DISABLED_AMFPropertyStorage_ValidateProperty) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}
////////////////////////////////////////////////////////////////////////

TEST_F(General, AMFData_Duplicate) {
	AMFBuffer* buffer;
	AMFBuffer* buffer_res;
	context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	res = buffer->Duplicate(AMF_MEMORY_HOST, (AMFData**) &buffer_res);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(General, DISABLED_AMFData_Convert) {
	AMFBuffer* buffer;
	AMFBuffer* buffer_res;
	context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	res = buffer->Convert(AMF_MEMORY_OPENCL);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_NE(buffer->GetDataType(), AMF_MEMORY_OPENCL);
}

TEST_F(General, DISABLED_AMFData_Interop) {
	AMFBuffer* buffer;
	AMFBuffer* buffer_res;
	context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	res = buffer->Interop(AMF_MEMORY_OPENCL);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(General, AMFBuffer_SetSize) {
	AMFBuffer* buffer;
	AMFBuffer* buffer_res;
	context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	res = buffer->SetSize((amf_size)2048);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(General, DISABLED_AMFSurface_SetCrop) {
	AMFSurface* surface;
	context1->AllocSurface(AMF_MEMORY_HOST, AMF_SURFACE_ARGB, 10, 10, &surface);
	res = surface->SetCrop(5, 5, 2, 2);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(General, AMFPrograms_RegisterKernelSource) {
	AMFPrograms* program;
	factory->GetPrograms(&program);
	AMF_KERNEL_ID kernel = 0;
	const char* kernel_src = "\n" \
		"__kernel void square2( __global float* input, __global float* output, \n" \
		" const unsigned int count) {            \n" \
		" int i = get_global_id(0);              \n" \
		" if(i < count) \n" \
		" output[i] = input[i] * input[i]; \n" \
		"}                     \n";
	program->RegisterKernelSource(&kernel, L"kernelIDName", "square2", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);
	EXPECT_FALSE(kernel);
}

// TODO: Add test files and finish test
TEST_F(General, AMFPrograms_RegisterKernelSourceFile) {
	AMFPrograms* program;
	factory->GetPrograms(&program);
	AMF_KERNEL_ID kernel = 0;
	EXPECT_NE(program->RegisterKernelSourceFile(&kernel, L"kenelIDname", "square2", L"test_shader.cl", NULL), AMF_NOT_IMPLEMENTED);
}

// TODO: Add a coresponding binary and finish test
TEST_F(General, AMFPrograms_RegisterKernelBinary) {
	AMFPrograms* program;
	factory->GetPrograms(&program);
	AMF_KERNEL_ID kernel = 0;
	EXPECT_NE(program->RegisterKernelBinary(&kernel, L"kenelIDname", "square2", NULL, NULL, NULL), AMF_NOT_IMPLEMENTED);
}

struct AMF_Rate : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFFactory* factory;
	//AMF_RESULT res;
	AMFRate rate1 = AMFConstructRate(7,12);
	AMFRate rate2;
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("AMFRate");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Rate() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		startTime = initiateTestLog();
	}

	~AMF_Rate() {
		context1.Release();
		terminateTestLog(startTime);
	}
};

TEST_F(AMF_Rate, SameAMFRate) {
	rate2 = AMFConstructRate(7, 12);
	bool resu = rate1.operator==(rate2);
	EXPECT_TRUE(resu);
}

TEST_F(AMF_Rate, SameFalseAMFRate) {
	rate2 = AMFConstructRate(8, 12);
	bool resu = rate1.operator==(rate2);
	EXPECT_FALSE(resu);
}

TEST_F(AMF_Rate, NotSameAMFRate) {
	rate2 = AMFConstructRate(42, -7);
	bool resu = rate1.operator!=(rate2);
	EXPECT_TRUE(resu);
}

TEST_F(AMF_Rate, NotSameFalseAMFRate) {
	rate2 = rate1;
	bool resu = rate1.operator!=(rate2);
	EXPECT_FALSE(resu);
}


struct AMF_Ratio : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFFactory* factory;
	//AMF_RESULT res;
	AMFRatio ratio1 = AMFConstructRatio(37, 3);
	AMFRatio ratio2;
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("AMFRatio");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Ratio() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		startTime = initiateTestLog();
	}

	~AMF_Ratio() {
		context1.Release();
		terminateTestLog(startTime);
	}
};

TEST_F(AMF_Ratio, SameAMFRatio) {
	ratio2 = AMFConstructRatio(37, 3);
	bool resu = ratio1.operator==(ratio2);
	EXPECT_TRUE(resu);
}

TEST_F(AMF_Ratio, SameFalseAMFRatio) {
	ratio2 = AMFConstructRatio(3, 37);
	bool resu = ratio1.operator==(ratio2);
	EXPECT_FALSE(resu);
}

TEST_F(AMF_Ratio, NotSameAMFRatio) {
	ratio2 = AMFConstructRatio(-1, 127);
	bool resu = ratio1.operator!=(ratio2);
	EXPECT_TRUE(resu);
}

TEST_F(AMF_Ratio, NotSameFalseAMFRatio) {
	ratio2 = ratio1;
	bool resu = ratio1.operator!=(ratio2);
	EXPECT_FALSE(resu);
}


struct AMF_Color : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFFactory* factory;
	//AMF_RESULT res;
	AMFColor color1 = AMFConstructColor(148, 52, 227, 1);
	AMFColor color2;
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("AMFColor");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Color() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		startTime = initiateTestLog();
	}

	~AMF_Color() {
		context1.Release();
		terminateTestLog(startTime);
	}
};

TEST_F(AMF_Color, SameAMFColor) {
	color2 = AMFConstructColor(148, 52, 227, 1);
	bool resu = color1.operator==(color2);
	EXPECT_TRUE(resu);
}

TEST_F(AMF_Color, SameFalseAMFColor) {
	color2 = AMFConstructColor(28, 127, 173, 7);
	bool resu = color1.operator==(color2);
	EXPECT_FALSE(resu);
}

TEST_F(AMF_Color, NotSameAMFColor) {
	color2 = AMFConstructColor(-28, 127, 173, 0);
	bool resu = color1.operator!=(color2);
	EXPECT_TRUE(resu);
}

TEST_F(AMF_Color, NotSameFalseAMFColor) {
	color2 = color1;
	bool resu = color1.operator!=(color2);
	EXPECT_FALSE(resu);
}
//todo
TEST_F(AMF_Color, AbnormalAMFColor) {
	color2 = AMFConstructColor(555, 124, 125, 1);
	EXPECT_ANY_THROW(color2);
}

struct AMF_Guid : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFFactory* factory;
	AMF_RESULT res;
	AMFGuid guid1 = AMFGuid(1, 2, 3, 41, 42, 43, 44, 45, 46, 47, 48);
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("AMFGuid");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Guid() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		startTime = initiateTestLog();
	}

	~AMF_Guid() {
		context1.Release();
		terminateTestLog(startTime);
	}
};

TEST_F(AMF_Guid, SameAMFGuid) {
	AMFGuid guid2 = AMFGuid(1, 2, 3, 41, 42, 43, 44, 45, 46, 47, 48);
	bool resu = guid1.operator==(guid2);
	EXPECT_TRUE(resu);
}

TEST_F(AMF_Guid, SameFalseAMFGuid) {
	AMFGuid guid2 = AMFGuid(100, 200, 300, 41, 42, 43, 44, 45, 46, 47, 48);
	bool resu = guid1.operator==(guid2);
	EXPECT_FALSE(resu);
}

TEST_F(AMF_Guid, NotSameAMFGuid) {
	AMFGuid guid2 = AMFGuid(1, 2, 3, 41, 42, 43, 44, 45, 46, 47, 4800000000);
	bool resu = guid1.operator!=(guid2);
	EXPECT_TRUE(resu);
}

TEST_F(AMF_Guid, NotSameFalseAMFGuid) {
	AMFGuid guid2 = guid1;
	bool resu = guid1.operator!=(guid2);
	EXPECT_FALSE(resu);
}

struct AMF_Variant : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFFactory* factory;
	AMFVariant var1 = AMFVariant((amf_bool) true);
	AMFVariant var2 = AMFVariant();
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("AMFVariant");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Variant() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		startTime = initiateTestLog();
	}

	~AMF_Variant() {
		context1.Release();
		terminateTestLog(startTime);
	}
};

TEST_F(AMF_Variant, SameAMFVariant) {
	AMFVariant var2 = AMFVariant(var1);
	bool resu = var1.operator==(var2);
	EXPECT_TRUE(resu);
}

TEST_F(AMF_Variant, SameFalseAMFVariant) {
	var1.operator=(5);
	var2 = 5.0;
	bool resu = var1.operator==(var2);
	EXPECT_FALSE(resu);
}

TEST_F(AMF_Variant, NotSameAMFVatiant) {
	var2 = AMFVariant(1);
	bool resu = var1.operator!=(var2);
	EXPECT_TRUE(resu);
}

TEST_F(AMF_Variant, NotSameFalseAMFVariant) {
	var2.operator=(var1);
	bool resu = var1.operator!=(var2);
	EXPECT_FALSE(resu);
}

TEST_F(AMF_Variant, EmptyAMFVariant) {
	bool resu = var1.Empty();
	EXPECT_FALSE(resu);
}

TEST_F(AMF_Variant, NotEmptyAMFVariant) {
	bool resu = var2.Empty();
	EXPECT_TRUE(resu);
}

TEST_F(AMF_Variant, ClearAMFVariant) {
	bool resu1 = var1.Empty();
	EXPECT_FALSE(resu1);
	var1.Clear();
	bool resu2 = var1.Empty();
	EXPECT_TRUE(resu2);
}

TEST_F(AMF_Variant, ConcersionAMFVariant) {
	EXPECT_EQ(var1.ToBool(), true);
} //писать ли прямо для всего??

TEST_F(AMF_Variant, CastAMFVariant) {
	EXPECT_EQ(var1.operator amf_double(), 1.0);
}

TEST_F(AMF_Variant, ChangeTypeItselfAMFVariant) {
	var1.ChangeType(AMF_VARIANT_EMPTY);
	bool resu = var1.Empty();
	EXPECT_TRUE(resu);
}

TEST_F(AMF_Variant, AttachAMFVariant) {
	AMFVariant var3 = AMFVariant();
	var3.Attach(var1);
	EXPECT_EQ(var3.operator amf_bool(), true);
}

TEST_F(AMF_Variant, DetachAMFVariant) {
	AMFVariant var3 = AMFVariant(var1.Detach());
	EXPECT_EQ(var3.operator amf_bool(), true);
}

TEST_F(AMF_Variant, StringAMFVariant) {
	AMFVariant::String str1;
	str1.operator=("A  M  F");
	EXPECT_EQ(str1, "A  M  F");
}


TEST_F(AMF_Variant, WStringAMFVariant) {
	AMFVariant::WString wstr1;
	wstr1.operator=(L"123");
	EXPECT_EQ(wstr1, L"123");
}

TEST_F(AMF_Variant, SameStringAMFVariant) {
	AMFVariant::String str1;
	AMFVariant::String str2;
	str1.operator=("A  M  F");
	str2 = "A  M  F";
	EXPECT_TRUE(str1.operator==(str2));
}

TEST_F(AMF_Variant, SameWStringAMFVariant) {
	AMFVariant::WString wstr1;
	AMFVariant::WString wstr2;
	wstr1.operator=(L"123");
	wstr2 = (L"123");
	EXPECT_TRUE(wstr1.operator==(wstr2));
}

TEST_F(AMF_Variant, SameStringFalseAMFVariant) {
	AMFVariant::String str1;
	AMFVariant::String str2;
	str1.operator=("A  M  F");
	str2 = "A M F";
	EXPECT_FALSE(str1.operator==(str2));
}

TEST_F(AMF_Variant, SameWStringFalseAMFVariant) {
	AMFVariant::WString wstr1;
	AMFVariant::WString wstr2;
	wstr1.operator=(L"123");
	wstr2 = (L"124");
	EXPECT_FALSE(wstr1.operator==(wstr2));
}

/*Нет оператора !=
TEST_F(AMF_Variant, NotSameStringAMFVariant) {
	AMFVariant::String str1;
	AMFVariant::String str2;
	str1.operator=("A  M  F");
	str2 = "AMF";
	EXPECT_TRUE(str1!=str2);
}*/


TEST_F(AMF_Variant, C_strStringAMFVariant) {
	AMFVariant::String str1;
	AMFVariant::String str0 = str1.c_str();
	EXPECT_EQ(str0[str0.size() - 1], '\x90');
}

TEST_F(AMF_Variant, C_strWStringAMFVariant) {
	AMFVariant::WString wstr1;
	AMFVariant::WString wstr0 = wstr1.c_str();
	EXPECT_EQ(wstr0[wstr0.size() - 1], L'\x9000');
}

TEST_F(AMF_Variant, SizeStringAMFVariant) {
	AMFVariant::String str1 = "AMF";
	EXPECT_EQ(str1.size(), 3);
}

TEST_F(AMF_Variant, SizeWStringAMFVariant) {
	AMFVariant::WString wstr1 = (L"AMF");
	EXPECT_EQ(wstr1.size(), 3);
}


struct AMF_PropertyStorage : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFFactory* factory;

	AMFPropertyStorage *ps;
	AMFVariantStruct* var1;
	AMF_RESULT AMF_CDECL_CALL resu1 = AMFVariantInit(var1);

	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("AMFPropertyStorage");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_PropertyStorage() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		startTime = initiateTestLog();
	}

	~AMF_PropertyStorage() {
		context1.Release();
		terminateTestLog(startTime);
	}
};


TEST_F(AMF_PropertyStorage, SetPropertyAMFPropertyStorage) {
	AMF_RESULT AMF_STD_CALL ares = ps->SetProperty(L"tru", var1);
	EXPECT_EQ(ares, AMF_OK);
}

TEST_F(AMF_PropertyStorage, GetPropertyOkAMFPropertyStorage) {
	AMF_RESULT AMF_STD_CALL ares0 = ps->SetProperty(L"tru", var1);
	AMF_RESULT AMF_STD_CALL ares = ps->GetProperty(L"tru", var1);
	EXPECT_EQ(ares, AMF_OK);
}

TEST_F(AMF_PropertyStorage, GetPropertyNotFoundAMFPropertyStorage) {
	AMF_RESULT AMF_STD_CALL ares = ps->GetProperty(L"fals", var1);
	EXPECT_EQ(ares, AMF_NOT_FOUND);
}

/* todo
TEST_F(AMF_PropertyStorage, GetPropertyStringOkAMFPropertyStorage) {
	AMF_RESULT AMF_STD_CALL ares0 = ps->SetProperty(L"tru", var1);
	AMF_RESULT AMF_STD_CALL ares = ps->GetPropertyString(L"tru", var1);
	EXPECT_EQ(ares, "AMF_OK");
}

TEST_F(AMF_PropertyStorage, GetPropertyStringNotFoundAMFPropertyStorage) {
	AMF_RESULT AMF_STD_CALL ares = ps->GetPropertyString(L"fals", var1);
	EXPECT_EQ(ares, "AMF_NOT_FOUND");
}

TEST_F(AMF_PropertyStorage, GetPropertyWStringOkAMFPropertyStorage) {
	AMF_RESULT AMF_STD_CALL ares0 = ps->SetProperty(L"tru", var1);
	AMF_RESULT AMF_STD_CALL ares = ps->GetPropertyWString(L"tru", var1);
	EXPECT_EQ(ares, L"AMF_OK");
}

TEST_F(AMF_PropertyStorage, GetPropertyWStringNotFoundAMFPropertyStorage) {
	AMF_RESULT AMF_STD_CALL ares = ps->GetPropertyWString(L"fals", var1);
	EXPECT_EQ(ares, L"AMF_NOT_FOUND");
}*/

TEST_F(AMF_PropertyStorage, HasPropertyAMFPropertyStorage) {
	bool AMF_STD_CALL hp = ps->HasProperty(L"tru");
	EXPECT_TRUE(hp);
}

TEST_F(AMF_PropertyStorage, GetPropertyCountAMFPropertyStorage) {
	amf_size AMF_STD_CALL res = ps->GetPropertyCount();
	EXPECT_EQ(res, 0);
	AMF_RESULT AMF_STD_CALL ares0 = ps->SetProperty(L"tru", var1);
	amf_size AMF_STD_CALL res2 = ps->GetPropertyCount();
	EXPECT_EQ(res, 1);
}

TEST_F(AMF_PropertyStorage, GetPropertyAtAMFPropertyStorage) {
	AMF_RESULT AMF_STD_CALL ares0 = ps->SetProperty(L"tru", var1);
	AMF_RESULT AMF_STD_CALL res = ps->GetPropertyAt(0, L"tru", 3, var1);
	EXPECT_EQ(res, 0);
}

TEST_F(AMF_PropertyStorage, ClearAMFPropertyStorage) {
	ps->SetProperty(L"tru", var1);
	bool AMF_STD_CALL hp = ps->HasProperty(L"tru");
	EXPECT_TRUE(hp);
	AMF_RESULT AMF_STD_CALL res = ps->Clear();
	hp = ps->HasProperty(L"tru");
	EXPECT_FALSE(hp);
}

TEST_F(AMF_PropertyStorage, AddToAMFPropertyStorage) {
	ps->SetProperty(L"tru", var1);
	AMFPropertyStorage* ps2;
	ps->AddTo(ps2, true, true);
	AMF_RESULT AMF_STD_CALL ares = ps2->GetProperty(L"tru", var1);
	EXPECT_EQ(ares, AMF_OK);
}

TEST_F(AMF_PropertyStorage, CopyToAMFPropertyStorage) {
	ps->SetProperty(L"tru", var1);
	AMFPropertyStorage* ps2;
	ps->CopyTo(ps2, true);
	AMF_RESULT AMF_STD_CALL ares = ps2->GetProperty(L"tru", var1);
	EXPECT_EQ(ares, AMF_OK);
}

//observer?


struct AMF_Mutex : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFFactory* factory;
	AMF_RESULT res;
	AMFMutex mutex1;
	AMFQueue<AMFColor> queue1;
	AMFColor color1 = AMFConstructColor(148, 52, 227, 1);
	AMFColor color2 = AMFConstructColor(-28, 127, 173, 0);
	AMFColor color3;
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("AMFMutex");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Mutex() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		startTime = initiateTestLog();
	}

	~AMF_Mutex() {
		context1.Release();
		terminateTestLog(startTime);
	}
};

TEST_F(AMF_Mutex, AMFMutexValid) {
	mutex1.Lock();
	EXPECT_EQ(mutex1.IsValid(), true);
	mutex1.Unlock();
	EXPECT_EQ(mutex1.IsValid(), true);
	mutex1.~AMFMutex();
	EXPECT_EQ(mutex1.IsValid(), FALSE);
}

TEST_F(AMF_Mutex, AMFMutexGet) {
	queue1.Add(0, color1);
	queue1.Add(0, color2);
	mutex1.Lock();
	amf_ulong front = 0;
	queue1.Get(front, color3, 300);
	mutex1.Unlock();
	EXPECT_EQ(color1, color3);
}

TEST_F(AMF_Mutex, AMFMutexDoubleLock) {
	queue1.Add(0, color1);
	queue1.Add(0, color2);
	bool firstLock = mutex1.Lock();
	bool canLock = mutex1.Lock();
	amf_ulong front = 0;
	queue1.Get(front, color3, 300);
	EXPECT_TRUE(firstLock);
	EXPECT_FALSE(canLock);
	mutex1.Unlock();
}

TEST_F(AMF_Mutex, AMFMutexDoubleUnlock) {
	queue1.Add(0, color1);
	queue1.Add(0, color2);
	mutex1.Lock();
	amf_ulong front = 0;
	queue1.Get(front, color3, 300);
	bool firstUnlock = mutex1.Unlock();
	bool canUnlock = mutex1.Unlock();
	EXPECT_TRUE(firstUnlock);
	EXPECT_FALSE(canUnlock);
}

struct AMF_Queue : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFFactory* factory;
	AMF_RESULT res;
	AMFQueue<AMFColor> queue1;
	AMFColor color1 = AMFConstructColor(148, 52, 227, 1);
	AMFColor color2 = AMFConstructColor(-28, 127, 173, 0);
	AMFColor color3;
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("AMFQueue");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Queue() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		startTime = initiateTestLog();
	}

	~AMF_Queue() {
		context1.Release();
		terminateTestLog(startTime);
	}
};

TEST_F(AMF_Queue, AMFQueueSize) {
	EXPECT_EQ(queue1.GetQueueSize(), 0);
	EXPECT_EQ(queue1.GetSize(), 0);
	queue1.Add(0, color1);
	queue1.Add(0, color2);
	EXPECT_EQ(queue1.GetQueueSize(), 0);
	EXPECT_EQ(queue1.GetSize(), 2);
}

TEST_F(AMF_Queue, AMFQueueQSize) {
	EXPECT_EQ(queue1.GetQueueSize(), 0);
	EXPECT_EQ(queue1.GetSize(), 0);
	amf_int32 siz = 3;
	queue1.SetQueueSize(siz);
	EXPECT_EQ(queue1.GetQueueSize(), 3);
	EXPECT_EQ(queue1.GetSize(), 0);
}

TEST_F(AMF_Queue, AMFQueueAddGet) {
	EXPECT_EQ(queue1.GetSize(), 0);
	queue1.Add(0, color1);
	queue1.Add(0, color2);
	EXPECT_EQ(queue1.GetSize(), 2);
	amf_ulong front = 0;
	queue1.Get(front, color3, 300);
	front++;
	AMFColor color4;
	queue1.Get(front, color4, 300);
	EXPECT_EQ(color1, color3);
	EXPECT_EQ(color2, color4);
}

TEST_F(AMF_Queue, AMFQueueClear) {
	EXPECT_EQ(queue1.GetSize(), 0);
	queue1.Add(0, color1);
	EXPECT_EQ(queue1.GetSize(), 1);
	queue1.Clear();
	EXPECT_EQ(queue1.GetSize(), 0);
}