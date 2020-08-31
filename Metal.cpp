#include "autotests.h"

struct Metal : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFComputeFactoryPtr computeFactory;
	AMFFactory* factory;
	int deviceCount;
	AMF_RESULT res;
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("Metal");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	Metal() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		context1->GetMetalComputeFactory(&computeFactory);
		context1->InitMetal();
		g_AMFFactory.Init();
		startTime = initiateTestLog();
	}

	~Metal() {
		context1.Release();
		computeFactory.Release();
		g_AMFFactory.Terminate();
		helper.Terminate();
		terminateTestLog(startTime);
	}
};

TEST_F(Metal, AMFContext_GetOpenCLComputeFactory) {
	AMFComputeFactoryPtr fact;
	res = context1->GetOpenCLComputeFactory(&fact);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFContext_InitDX11_GetDX11Device) {
	res = context1->InitDX11(context1->GetDX11Device());
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFContext_InitDX9_GetDX9Device) {
	res = context1->InitDX9(context1->GetDX9Device());
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFContext_InitOpenGL_GetOpenGLContext) {
	res = context1->InitOpenGL(context1->GetOpenGLContext(), NULL, NULL);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFContext_InitXV_GetXVDevice) {
	res = context1->InitXV(context1->GetXVDevice());
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFContext_InitGralloc_GetGrallocDevice) {
	res = context1->InitGralloc(context1->GetGrallocDevice());
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFContext_LockOpenCL_UnlockOpenCL) {
	res = context1->LockOpenCL();
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	res = context1->UnlockOpenCL();
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

////////////////////////////////////////////////////////////////////////////////////

TEST_F(Metal, AMFFactory_CreateContext) {
	res = factory->CreateContext(&context1);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFFactory_CreateComponent) {
	AMFComponentPtr component;
	res = factory->CreateComponent(context1, 0, &component);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFFactory_SetCacheFolder) {
	res = factory->SetCacheFolder(L"test");
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFFactory_GetDebug) {
	AMFDebug* debug;
	res = factory->GetDebug(&debug);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFFactory_GetTrace) {
	AMFTrace* trace;
	res = factory->GetTrace(&trace);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFFactory_GetPrograms) {
	AMFPrograms* programs;
	res = factory->GetPrograms(&programs);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}


TEST_F(Metal, AMFTrace_SetPath) {
	AMFTrace* trace;
	factory->GetTrace(&trace);
	res = trace->SetPath(L"test");
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFTrace_GetPath) {
	AMFTrace* trace;
	factory->GetTrace(&trace);
	amf_size* size;
	*size = (amf_size)30;
	res = trace->GetPath(new wchar_t[1000], size);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFVariantStruct_AMFVariantInit) {
	AMFVariantStruct variant;
	res = AMFVariantInit(&variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFVariantStruct_AMFVariantClear) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantClear(&variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFVariantStruct_AMFVariantAssignBool) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantAssignBool(&variant, true);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetBool(&variant), true);
}

TEST_F(Metal, AMFVariantStruct_AMFVariantAssignInt64) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantAssignInt64(&variant, (amf_int64) 4);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetInt64(&variant), (amf_int64) 4);
}

TEST_F(Metal, AMFVariantStruct_AMFVariantAssignDouble) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetDouble(&variant), (amf_double) 2.74);
}

TEST_F(Metal, AMFVariantStruct_AMFVariantAssignString) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantAssignString(&variant, "ТеStパーティーへ行かないか");
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(*AMFVariantGetString(&variant), *"ТеStパーティーへ行かないか");
}

TEST_F(Metal, AMFVariantStruct_AMFVariantAssignWString) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	res = AMFVariantAssignWString(&variant, L"ТеStパーティーへ行かないか");
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(*AMFVariantGetWString(&variant), *L"ТеStパーティーへ行かないか");
}

TEST_F(Metal, AMFVariantStruct_AMFVariantAssignInterface) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFInterface* inter = NULL;
	res = AMFVariantAssignInterface(&variant, inter);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFVariantStruct_AMFVariantAssignRect) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFRect* rect = new AMFRect;
	res = AMFVariantAssignRect(&variant, rect);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetRect(&variant), *rect);
}

TEST_F(Metal, AMFVariantStruct_AMFVariantAssignSize) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFSize* size = new AMFSize;
	res = AMFVariantAssignSize(&variant, size);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetSize(&variant), *size);
}

TEST_F(Metal, AMFVariantStruct_AMFVariantAssignPoint) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFPoint* point = new AMFPoint;
	res = AMFVariantAssignPoint(&variant, point);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetPoint(&variant), *point);
}

TEST_F(Metal, AMFVariantStruct_AMFVariantAssignRate) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFRate* rate = new AMFRate;
	res = AMFVariantAssignRate(&variant, rate);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetRate(&variant), *rate);
}

TEST_F(Metal, AMFVariantStruct_AMFVariantAssignRatio) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFRatio* ratio = new AMFRatio;
	res = AMFVariantAssignRatio(&variant, ratio);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetRatio(&variant), *ratio);
}

TEST_F(Metal, AMFVariantStruct_AMFVariantAssignColor) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFColor* color = new AMFColor;
	color->r = 1; color->g = 0.5; color->b = 0; color->a = 0;
	res = AMFVariantAssignColor(&variant, color);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(AMFVariantGetColor(&variant), *color);
}

TEST_F(Metal, AMFVariantStruct_AMFVariantCompare) {
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

TEST_F(Metal, AMFVariantStruct_AMFVariantCopy) {
	AMFVariantStruct variant1, variant2;
	AMFVariantInit(&variant1); AMFVariantInit(&variant2);
	AMFColor* color = new AMFColor;
	color->r = 1; color->g = 0.5; color->b = 0; color->a = 0;
	AMFVariantAssignColor(&variant2, color);
	res = AMFVariantCopy(&variant1, &variant2);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFVariantStruct_AMFVariantChangeType) {
	AMFVariantStruct variant1, variant2;
	AMFVariantInit(&variant1);
	amf_int64 in = 1;
	AMFVariantAssignInt64(&variant1, in);
	amf_double fl = 2.2;
	AMFVariantAssignDouble(&variant2, fl);
	res = AMFVariantChangeType(&variant2, &variant1, AMF_VARIANT_DOUBLE);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFPropertyStorage_SetProperty) {
	AMFVariantStruct variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	res = context1->SetProperty(L"TestProperty", variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFPropertyStorage_GetProperty) {
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


TEST_F(Metal, AMFPropertyStorage_GetPropertyString) {
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignInt64(&variant, (amf_int64) 2);
	context1->SetProperty(L"TestProperty", variant);
	const char* res_str;
	res = context1->GetPropertyString(L"TestProperty", &res_str);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_STREQ(res_str, "2");
}

TEST_F(Metal, AMFPropertyStorage_GetPropertyWString) {
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
TEST_F(Metal, DISABLED_AMFPropertyStorage_GetPropertyAt) {
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	context1->SetProperty(L"TestProperty", variant);
	res = context1->GetPropertyAt((amf_size) 0, L"TestProperty", 12, &res_variant);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(res_variant.doubleValue, 2.74);
}

TEST_F(Metal, AMFPropertyStorage_Clear) {
	AMFVariantStruct variant, res_variant;
	AMFVariantInit(&variant);
	AMFVariantAssignDouble(&variant, (amf_double) 2.74);
	context1->SetProperty(L"TestProperty", variant);
	context1->Clear();
	res = context1->GetProperty(L"TestProperty", &res_variant);
	EXPECT_EQ(res, AMF_NOT_FOUND);
}

TEST_F(Metal, AMFPropertyStorage_AddTo) {
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

TEST_F(Metal, AMFPropertyStorage_CopyTo) {
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
TEST_F(Metal, DISABLED_AMFPropertyStorageEx_GetPropertyInfo) {
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

TEST_F(Metal, DISABLED_AMFPropertyStorage_ValidateProperty) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}
////////////////////////////////////////////////////////////////////////

TEST_F(Metal, AMFData_Duplicate) {
	AMFBuffer* buffer;
	AMFBuffer* buffer_res;
	context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	res = buffer->Duplicate(AMF_MEMORY_HOST, (AMFData**) &buffer_res);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFData_Convert) {
	AMFBuffer* buffer;
	AMFBuffer* buffer_res;
	context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	res = buffer->Convert(AMF_MEMORY_OPENCL);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFData_Interop) {
	AMFBuffer* buffer;
	AMFBuffer* buffer_res;
	context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	res = buffer->Interop(AMF_MEMORY_OPENCL);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFBuffer_SetSize) {
	AMFBuffer* buffer;
	AMFBuffer* buffer_res;
	context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	res = buffer->SetSize((amf_size)2048);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFSurface_SetCrop) {
	AMFSurface* surface;
	context1->AllocSurface(AMF_MEMORY_HOST, AMF_SURFACE_ARGB, 10, 10, &surface);
	res = surface->SetCrop(5, 5, 2, 2);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFComputeFactory_GetDeviceAt) {
	AMFComputeDevice* device;
	EXPECT_EQ(computeFactory->GetDeviceAt(0, &device), AMF_OK);
	EXPECT_TRUE(device);
}

TEST_F(Metal, AMFDeviceCompute_CreateCompute) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	EXPECT_EQ(device->CreateCompute(nullptr, &pCompute), AMF_OK);
	EXPECT_TRUE(pCompute);
}

TEST_F(Metal, AMFDeviceCompute_CreateComputeEx) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	EXPECT_EQ(device->CreateComputeEx(nullptr, &pCompute), AMF_OK);
	EXPECT_TRUE(pCompute);
}

// TODO: Add test files and finish test
TEST_F(Metal, AMFPrograms_RegisterKernelSourceFile) {
	AMFPrograms* program;
	factory->GetPrograms(&program);
	AMF_KERNEL_ID kernel = 0;
	EXPECT_NE(program->RegisterKernelSourceFile(&kernel, L"kenelIDname", "square2", L"test_shader.cl", NULL), AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFPrograms_RegisterKernelSource) {
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
	AMFComputeKernelPtr pKernel;
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	EXPECT_EQ(pCompute->GetKernel(kernel, &pKernel), AMF_OK);
	EXPECT_TRUE(pKernel);
}

// TODO: Add a coresponding binary and finish test
TEST_F(Metal, AMFPrograms_RegisterKernelBinary) {
	AMFPrograms* program;
	factory->GetPrograms(&program);
	AMF_KERNEL_ID kernel = 0;
	EXPECT_NE(program->RegisterKernelBinary(&kernel, L"kenelIDname", "square2", NULL, NULL, NULL), AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFCompute_GetKernel) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
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
	amf::AMFComputeKernelPtr pKernel;
	EXPECT_EQ(pCompute->GetKernel(kernel, &pKernel), AMF_OK);
	EXPECT_TRUE(pKernel);
}

TEST_F(Metal, AMFCompute_PutSyncPoint) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFComputeSyncPointPtr sync;
	EXPECT_NE(pCompute->PutSyncPoint(&sync), AMF_NOT_IMPLEMENTED);
	EXPECT_TRUE(sync);
}

TEST_F(Metal, AMFCompute_FlushQueue) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	EXPECT_EQ(device->CreateCompute(nullptr, &pCompute), AMF_OK);
	EXPECT_NO_THROW(pCompute->FlushQueue());
}

TEST_F(Metal, AMFCompute_FinishQueue) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	EXPECT_NO_THROW(pCompute->FinishQueue());
}

TEST_F(Metal, DISABLED_AMFCompute_FillPlane) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_OPENCL, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	amf_size origin[3] = { 0, 0, 0 };
	amf_size region[3] = { 1, 1, 0 };
	float color[4] = { 1, 1, 0, 0 };
	EXPECT_NE(pCompute->FillPlane(plane, origin, region, color), AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFCompute_FillBuffer) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	EXPECT_TRUE(pCompute->GetMemoryType());
}

TEST_F(Metal, DISABLED_AMFCompute_ConvertPlaneToBuffer) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_OPENCL, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	AMFBufferPtr buffer;
	EXPECT_EQ(pCompute->ConvertPlaneToBuffer(plane, &buffer), AMF_OK);
	EXPECT_TRUE(buffer);
}

// TODO: Implement
TEST_F(Metal, DISABLED_AMFCompute_CopyBuffer) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	EXPECT_TRUE(pCompute->GetMemoryType());
}

// SEH exception, find null
TEST_F(Metal, DISABLED_AMFCompute_CopyPlane) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_OPENCL, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	AMFPlanePtr plane2;
	amf_size origin[3] = { 0, 0, 0 };
	amf_size region[3] = { 1, 1, 0 };
	float color[4] = { 1, 1, 0, 0 };
	EXPECT_EQ(pCompute->CopyPlane(plane, origin, region, plane2, origin), AMF_OK);
	EXPECT_TRUE(plane2);
}

TEST_F(Metal, AMFCompute_CopyBufferToHost) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFBufferPtr buffer;
	context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024, &buffer);
	void* dest = malloc(1024);
	pCompute->CopyBufferToHost(buffer, 0, 1024, dest, true);
	EXPECT_TRUE(dest);
}

TEST_F(Metal, AMFCompute_CopyBufferFromHost) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFBufferPtr buffer;
	context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024, &buffer);
	void* dest = malloc(1024);
	pCompute->CopyBufferToHost(buffer, 0, 1024, dest, true);
	AMFBufferPtr buffer2;
	context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024, &buffer2);
	pCompute->CopyBufferFromHost(dest, 1024, buffer2, 0, true);
	EXPECT_TRUE(buffer2);
}

TEST_F(Metal, DISABLED_AMFCompute_CopyPlaneToHost) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_OPENCL, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	amf_size origin[3] = { 0, 0, 0 };
	amf_size region[3] = { 1, 1, 0 };
	float color[4] = { 1, 1, 0, 0 };
	void* dest = malloc(1024);
	pCompute->CopyPlaneToHost(plane, origin, region, dest, 1024, true);
	EXPECT_TRUE(dest);
}

TEST_F(Metal, DISABLED_AMFCompute_CopyPlaneFromHost) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_OPENCL, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	amf_size origin[3] = { 0, 0, 0 };
	amf_size region[3] = { 1, 1, 0 };
	float color[4] = { 1, 1, 0, 0 };
	void* dest = malloc(1024);
	pCompute->CopyPlaneToHost(plane, origin, region, dest, 1024, true);
	AMFPlanePtr plane2;
	pCompute->CopyPlaneFromHost(dest, origin, region, 1024, plane2, true);
	EXPECT_TRUE(plane2);
}

TEST_F(Metal, DISABLED_AMFCompute_ConvertPlaneToPlane) {
	AMFComputeDevice* device;
	computeFactory->GetDeviceAt(0, &device);
	AMFComputePtr pCompute;
	device->CreateCompute(nullptr, &pCompute);
	AMFSurfacePtr surface;
	context1->AllocSurface(AMF_MEMORY_OPENCL, AMF_SURFACE_RGBA, 2, 2, &surface);
	AMFPlanePtr plane = surface->GetPlane(AMF_PLANE_PACKED);
	AMFPlanePtr plane2;
	pCompute->ConvertPlaneToPlane(plane, &plane2, AMF_CHANNEL_ORDER_R, AMF_CHANNEL_UNSIGNED_INT32);
	EXPECT_TRUE(plane2);
}

// Causes crash, needs to be investigated
TEST_F(Metal, DISABLED_AMFComputeKernel_GetCompileWorkgroupSize) {
	AMFPrograms* pPrograms;
	factory->GetPrograms(&pPrograms);
	AMF_KERNEL_ID kernel = 0;
	const char* kernel_src = "\n" \
		"__kernel void square2( __global float* output, __global float* input, \n" \
		" const unsigned int count) {            \n" \
		" int i = get_global_id(0);              \n" \
		" if(i < count) \n" \
		" output[i] = input[i] * input[i]; \n" \
		"}                     \n";
	pPrograms->RegisterKernelSource(&kernel, L"kernelIDName", "multiplication", strlen(kernel_src), (amf_uint8*)kernel_src, NULL); 
	AMFComputeDevicePtr pComputeDevice;
	computeFactory->GetDeviceAt(0, &pComputeDevice);
	pComputeDevice->GetNativeContext();

	AMFComputePtr pCompute;
	pComputeDevice->CreateCompute(nullptr, &pCompute);

	AMFComputeKernelPtr pKernel;
	pCompute->GetKernel(kernel, &pKernel);

	amf_size* size = new amf_size;
	*size = (amf_size) 64;

	res = pKernel->GetCompileWorkgroupSize(size);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, AMFComputeKernel_Enqueue) {
	AMFPrograms* pPrograms;
	factory->GetPrograms(&pPrograms);
	AMF_KERNEL_ID kernel = 0;
	const char* kernel_src = "\n" \
		"__kernel void square2( __global float* output, __global float* input, \n" \
		" const unsigned int count) {            \n" \
		" int i = get_global_id(0);              \n" \
		" if(i < count) \n" \
		" output[i] = input[i] * input[i]; \n" \
		"}                     \n";
	pPrograms->RegisterKernelSource(&kernel, L"kernelIDName", "multiplication", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);
	AMFComputeDevicePtr pComputeDevice;
	computeFactory->GetDeviceAt(0, &pComputeDevice);
	pComputeDevice->GetNativeContext();

	AMFComputePtr pCompute;
	pComputeDevice->CreateCompute(nullptr, &pCompute);

	AMFComputeKernelPtr pKernel;
	pCompute->GetKernel(kernel, &pKernel);

	amf_size sizeLocal[3] = { 1024, 0, 0 };
	amf_size sizeGlobal[3] = { 1024, 0, 0 };
	amf_size offset[3] = { 0, 0, 0 };

	res = pKernel->Enqueue(1, offset, sizeGlobal, sizeLocal);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_Init) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_ReInit) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_Terminate) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_SubmitInput) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_QueryOutput) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_Drain) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_Flush) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_SetOutputDataAllocatorCB) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFComponent_GetCaps) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFCaps_GetInputCaps) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFCaps_GetOutputCaps) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFIOCaps_GetFormatAt) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFIOCaps_GetMemoryTypeAt) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFDataAllocatorCB_AllocBuffer) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}

TEST_F(Metal, DISABLED_AMFDataAllocatorCB_AllocSurface) {
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
}
