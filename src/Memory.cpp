#include "autotests.h"


struct AMF_Data : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFFactory* factory;
	AMF_RESULT res;
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_Data");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Data() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		startTime = initiateTestLog();
	}

	~AMF_Data() {
		context1.Release();
		terminateTestLog(startTime);
	}
};


/*TEST_F(AMF_Data, getDataTypeFromMemoryUnknown) {
	AMFBuffer* buffer;
	context1->AllocBuffer(AMF_MEMORY_UNKNOWN, 1024 * sizeof(float), &buffer);
	EXPECT_ANY_THROW(buffer->GetDataType());
}*/

TEST_F(AMF_Data, getExpectedDataBufferType) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(buffer->GetDataType(), AMF_DATA_BUFFER);
}

TEST_F(AMF_Data, getExpectedDataSurfaceType) {
	AMFSurface* surface;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &surface);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(surface->GetDataType(), AMF_DATA_SURFACE);
}

TEST_F(AMF_Data, getExpectedDataAudioBufferType) {
	AMFAudioBuffer* aubuf;
	res = context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &aubuf);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(aubuf->GetDataType(), AMF_DATA_AUDIO_BUFFER);
}

/*TEST_F(AMF_Data, getExpectedDataUserType) {
	EXPECT_EQ(->GetDataType(), AMF_DATA_USER);
}*/

TEST_F(AMF_Data, getExpectedMemoryHostType) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(buffer->GetMemoryType(), AMF_MEMORY_HOST);
}

TEST_F(AMF_Data, memoryHostBlockDuplicate) {
	AMFBuffer* buffer;
	AMFBuffer* buffer_res;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	res = buffer->Duplicate(AMF_MEMORY_HOST, (AMFData**)&buffer_res);
	EXPECT_EQ(res, AMF_OK);
}


TEST_F(AMF_Data, DISABLED_convertMemoryHostToMemoryOpencl) {
	AMFBuffer* buffer;
	AMFBuffer* buffer_res;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	res = buffer->Convert(AMF_MEMORY_OPENCL);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(buffer->GetDataType(), AMF_MEMORY_OPENCL);
}

TEST_F(AMF_Data, interopMemoryHostToMemoryOpencl) {
	AMFBuffer* buffer;
	AMFBuffer* buffer_res;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	res = buffer->Interop(AMF_MEMORY_OPENCL);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(AMF_Data, checkDataObjectIsReusable) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(buffer->IsReusable(), true);
}

TEST_F(AMF_Data, DISABLED_checkDataObjectIsNotReusable) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_DX11, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(buffer->IsReusable(), false);
}

TEST_F(AMF_Data, setPtsDontCrush) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NO_THROW(buffer->SetPts(3000));
}

TEST_F(AMF_Data, getPtsDontCrush) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NO_THROW(buffer->GetPts());
}

TEST_F(AMF_Data, getPtsWithSetPtsChecking) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	buffer->SetPts(3333);
	EXPECT_EQ(buffer->GetPts(), 3333);
}

TEST_F(AMF_Data, setDurationDontCrush) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NO_THROW(buffer->SetDuration(3000));
}

TEST_F(AMF_Data, getDurationDontCrush) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NO_THROW(buffer->GetDuration());
}

TEST_F(AMF_Data, getDurationWithSetDurationChecking) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	buffer->SetDuration(3333);
	EXPECT_EQ(buffer->GetDuration(), 3333);
}


struct Memory : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFFactory* factory;
	AMF_RESULT res;
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("Memory");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	Memory() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		startTime = initiateTestLog();
	}

	~Memory() {
		context1.Release();
		terminateTestLog(startTime);
	}
};

TEST_F(Memory, AMFFactory_CreateContext) {
	res = factory->CreateContext(&context1);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_TRUE(context1);
}

TEST_F(Memory, DISABLED_AMFFactory_CreateComponent) {
	AMFComponentPtr component;
	res = factory->CreateComponent(context1, 0, &component);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_TRUE(component);
}

TEST_F(Memory, AMFFactory_SetCacheFolder) {
	res = factory->SetCacheFolder(L"test");
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(factory->GetCacheFolder(), L"test");
}

TEST_F(Memory, AMFFactory_GetDebug) {
	AMFDebug* debug;
	res = factory->GetDebug(&debug);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_TRUE(debug);
}

TEST_F(Memory, AMFFactory_GetTrace) {
	AMFTrace* trace;
	res = factory->GetTrace(&trace);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_TRUE(trace);
}

TEST_F(Memory, AMFFactory_GetPrograms) {
	AMFPrograms* programs;
	res = factory->GetPrograms(&programs);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_TRUE(programs);
}


TEST_F(Memory, AMFTrace_SetPath) {
	AMFTrace* trace;
	factory->GetTrace(&trace);
	res = trace->SetPath(L"test");
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(Memory, DISABLED_AMFTrace_GetPath) {
	AMFTrace* trace;
	res = factory->GetTrace(&trace);
	EXPECT_EQ(res, AMF_OK);
	res = trace->SetPath(L"test");
	amf_size* size;
	*size = (amf_size)30;
	wchar_t* str = new wchar_t[1000];
	res = trace->GetPath(str, size);
	EXPECT_EQ(str, L"test");
}

TEST_F(Memory, AMFVariantStruct_AMFVariantInit) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(Memory, AMFVariantStruct_AMFVariantClear) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignString(&variant, "ТеStパーティーへ行かないか");
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantClear(&variant);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(Memory, AMFVariantStruct_AMFVariantAssignBool) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignBool(&variant, true);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetBool(&variant), true);
}

TEST_F(Memory, AMFVariantStruct_AMFVariantAssignInt64) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignInt64(&variant, (amf_int64)4);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetInt64(&variant), (amf_int64)4);
}

TEST_F(Memory, AMFVariantStruct_AMFVariantAssignDouble) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignDouble(&variant, (amf_double)2.74);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetDouble(&variant), (amf_double)2.74);
}

TEST_F(Memory, AMFVariantStruct_AMFVariantAssignString) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignString(&variant, "ТеStパーティーへ行かないか");
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(*AMFVariantGetString(&variant), *"ТеStパーティーへ行かないか");
}

TEST_F(Memory, AMFVariantStruct_AMFVariantAssignWString) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignWString(&variant, L"ТеStパーティーへ行かないか");
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(*AMFVariantGetWString(&variant), *L"ТеStパーティーへ行かないか");
}

TEST_F(Memory, AMFVariantStruct_AMFVariantAssignInterface) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	AMFInterface* inter = NULL;
	res = AMFVariantAssignInterface(&variant, inter);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(Memory, AMFVariantStruct_AMFVariantAssignRect) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	AMFRect* rect = new AMFRect;
	res = AMFVariantAssignRect(&variant, rect);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetRect(&variant), *rect);
}

TEST_F(Memory, AMFVariantStruct_AMFVariantAssignSize) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	AMFSize* size = new AMFSize;
	res = AMFVariantAssignSize(&variant, size);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetSize(&variant), *size);
}

TEST_F(Memory, AMFVariantStruct_AMFVariantAssignPoint) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	AMFPoint* point = new AMFPoint;
	res = AMFVariantAssignPoint(&variant, point);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetPoint(&variant), *point);
}


TEST_F(Memory, AMFVariantStruct_AMFVariantAssignRatio) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	AMFRatio* ratio = new AMFRatio;
	res = AMFVariantAssignRatio(&variant, ratio);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetRatio(&variant), *ratio);
}

TEST_F(Memory, AMFVariantStruct_AMFVariantAssignColor) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	AMFColor* color = new AMFColor;
	color->r = 1; color->g = 0.5; color->b = 0; color->a = 0;
	res = AMFVariantAssignColor(&variant, color);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetColor(&variant), *color);
}

TEST_F(Memory, AMFVariantStruct_AMFVariantCompare) {
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

TEST_F(Memory, AMFVariantStruct_AMFVariantCopy) {
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

TEST_F(Memory, AMFVariantStruct_AMFVariantChangeType) {
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
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(AMFVariantGetType(&variant1), AMF_VARIANT_DOUBLE);
	EXPECT_EQ(AMFVariantGetType(&variant2), AMF_VARIANT_DOUBLE);
}

TEST_F(Memory, AMFPropertyStorage_SetProperty) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignDouble(&variant, (amf_double)2.74);
	EXPECT_EQ(res, AMF_OK);
	res = context1->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(Memory, AMFPropertyStorage_GetProperty) {
	AMFVariantStruct variant, res_variant;
	res = AMFVariantInit(&variant);
	EXPECT_EQ(res, AMF_OK);
	res = AMFVariantAssignDouble(&variant, (amf_double)2.74);
	EXPECT_EQ(res, AMF_OK);
	res = context1->SetProperty(L"TestProperty", variant);
	EXPECT_EQ(res, AMF_OK);
	res = context1->GetProperty(L"TestProperty", &res_variant);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
	res = context1->GetProperty(L"NotAProperty", &res_variant);
	EXPECT_EQ(res, AMF_NOT_FOUND);
}


TEST_F(Memory, AMFPropertyStorage_GetPropertyString) {
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignInt64(&variant, (amf_int64)2);
	context1->SetProperty(L"TestProperty", variant);
	const char* res_str;
	res = context1->GetPropertyString(L"TestProperty", &res_str);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_STREQ(res_str, "2");
}

TEST_F(Memory, AMFPropertyStorage_GetPropertyWString) {
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
TEST_F(Memory, DISABLED_AMFPropertyStorage_GetPropertyAt) {
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double)2.74);
	context1->SetProperty(L"TestProperty", variant);
	res = context1->GetPropertyAt((amf_size)0, L"TestProperty", 12, &res_variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
}

TEST_F(Memory, AMFPropertyStorage_Clear) {
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double)2.74);
	context1->SetProperty(L"TestProperty", variant);
	context1->Clear();
	res = context1->GetProperty(L"TestProperty", &res_variant);
	EXPECT_EQ(res, AMF_NOT_FOUND);
}

TEST_F(Memory, AMFPropertyStorage_AddTo) {
	AMFContextPtr context2;
	factory->CreateContext(&context2);
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double)2.74);
	context1->SetProperty(L"TestProperty", variant);
	res = context1->AddTo(context2, true, true);
	context2->GetProperty(L"TestProperty", &res_variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
}

TEST_F(Memory, AMFPropertyStorage_CopyTo) {
	AMFContextPtr context2;
	factory->CreateContext(&context2);
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double)2.74);
	context1->SetProperty(L"TestProperty", variant);
	res = context1->CopyTo(context2, true);
	context2->GetProperty(L"TestProperty", &res_variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
}

// TODO: Use AMF(Component/Input/Output) to implement the following 2 cases when CreateComponent is implemented
TEST_F(Memory, DISABLED_AMFPropertyStorageEx_GetPropertyInfo) {
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double)2.74);
	//context1->SetProperty(L"TestProperty", variant);
	AMFComponent* component;
	const AMFPropertyInfo** info = NULL;
	factory->CreateComponent(context1, L"component", &component);
	component->SetProperty(L"TestProperty", variant);
	res = component->GetPropertyInfo(L"TestProperty", info);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Memory, DISABLED_AMFPropertyStorage_ValidateProperty) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}
////////////////////////////////////////////////////////////////////////



TEST_F(Memory, AMFBuffer_SetSize) {
	AMFBuffer* buffer;
	AMFBuffer* buffer_res;
	context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	res = buffer->SetSize((amf_size)2048);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Memory, DISABLED_AMFSurface_SetCrop) {
	AMFSurface* surface;
	context1->AllocSurface(AMF_MEMORY_HOST, AMF_SURFACE_ARGB, 10, 10, &surface);
	res = surface->SetCrop(5, 5, 2, 2);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Memory, AMFPrograms_RegisterKernelSource) {
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
TEST_F(Memory, AMFPrograms_RegisterKernelSourceFile) {
	AMFPrograms* program;
	factory->GetPrograms(&program);
	AMF_KERNEL_ID kernel = 0;
	EXPECT_NE(program->RegisterKernelSourceFile(&kernel, L"kenelIDname", "square2", L"test_shader.cl", NULL), AMF_NOT_IMPLEMENTED);
}

// TODO: Add a coresponding binary and finish test
TEST_F(Memory, AMFPrograms_RegisterKernelBinary) {
	AMFPrograms* program;
	factory->GetPrograms(&program);
	AMF_KERNEL_ID kernel = 0;
	EXPECT_NE(program->RegisterKernelBinary(&kernel, L"kenelIDname", "square2", NULL, NULL, NULL), AMF_NOT_IMPLEMENTED);
}