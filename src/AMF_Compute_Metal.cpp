#ifdef __APPLE__
#include "autotests.h"

static AMFFactoryHelper helper;
static AMFContextPtr context;
static AMFComputeFactoryPtr metalComputeFactory;
static AMFFactory* factory;
static AMF_RESULT res;
static AMFPrograms* pPrograms;
static AMFComputeDevicePtr pComputeDevice;
static AMF_KERNEL_ID kernel = 0;
static AMFComputePtr pCompute;
static AMFComputeKernelPtr pKernel;
static AMFBuffer* input = NULL;
static AMFBuffer* input2 = NULL;
static AMFBuffer* output = NULL;
static float* inputData;
static float* inputData2;
static float* expectedData = new float[1024];
static int deviceCount;
static amf_size sizeLocal[3] = { 1024, 0, 0 };
static amf_size sizeGlobal[3] = { 1024, 0, 0 };
static amf_size offset[3] = { 0, 0, 0 };
static float* outputData2 = NULL;
static const char* kernel_src = "\n" \
"__kernel void square2( __global float* output, __global float* input, \n" \
" const unsigned int count) {            \n" \
" int i = get_global_id(0);              \n" \
" if(i < count) \n" \
" output[i] = input[i] * input[i]; \n" \
"}                     \n" \
"__kernel void multiplication(__global float* output, __global float* input, __global float* input2, \n" \
" const unsigned int count) {            \n" \
" int i = get_global_id(0);              \n" \
" if(i < count) \n" \
" output[i] = input[i] * input2[i]; \n" \
"}                     \n";
static chrono::time_point<chrono::system_clock> startTime;

struct AMF_Compute_Metal : testing::Test {
	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_Compute_Metal");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Compute_Metal() {
		startTime = initiateTestLog();
	}

	~AMF_Compute_Metal() {
		terminateTestLog(startTime);
	}
};

TEST_F(AMF_Compute_Metal, 1_InitializateFactory_Metal) {
	res = g_AMFFactory.Init();
	EXPECT_EQ(res, AMF_OK);
	res = helper.Init();
	EXPECT_EQ(res, AMF_OK);
	factory = helper.GetFactory();
}

TEST_F(AMF_Compute_Metal, 2_CreateContext_Metal) {
	res = factory->CreateContext(&context);
	EXPECT_EQ(res, AMF_OK);
	res = context->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	EXPECT_EQ(res, AMF_OK);
	res = context->GetMetalComputeFactory(&metalComputeFactory);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_Metal, 3_CreatePrograms_Metal) {
	res = factory->GetPrograms(&pPrograms);
	EXPECT_EQ(res, AMF_OK);
	res = pPrograms->RegisterKernelSource(&kernel, L"kernelIDName", "multiplication", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_Metal, 4_InitializeDevice_Metal) {
	res = metalComputeFactory->GetDeviceAt(0, &pComputeDevice);
	EXPECT_EQ(res, AMF_OK);
	pComputeDevice->GetNativeContext();
}

TEST_F(AMF_Compute_Metal, 5_GetComputeFromDeviceAndLoadKernel_Metal) {
	res = pComputeDevice->CreateCompute(nullptr, &pCompute);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_Metal, 6_LoadKernelIntoCompute_Metal) {
	res = pCompute->GetKernel(kernel, &pKernel);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_Metal, 7_InitMetalInContext_Metal) {
	res = context->InitMetal();
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_Metal, 8_AllocateBuffers_Metal) {
	res = context->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &input);
	EXPECT_EQ(res, AMF_OK);
	res = context->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &input2);
	EXPECT_EQ(res, AMF_OK);
	res = context->AllocBuffer(AMF_MEMORY_METAL, 1024 * sizeof(float), &output);
	EXPECT_EQ(res, AMF_OK);
	inputData = static_cast<float*>(input->GetNative());
	inputData2 = static_cast<float*>(input2->GetNative());
}

TEST_F(AMF_Compute_Metal, 9_InitializeRandomData_Metal) {
	for (int k = 0; k < 1024; k++)
	{
		inputData[k] = rand() / 50.00;
		inputData2[k] = rand() / 50.00;
		expectedData[k] = inputData[k] * inputData2[k];
	}
}

TEST_F(AMF_Compute_Metal, 10_ConvertInputFromHostToMetal_Metal) {
	res = input->Convert(AMF_MEMORY_METAL);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_Metal, 11_SetShaderArguments_Metal) {
	res = pKernel->SetArgBuffer(0, output, AMF_ARGUMENT_ACCESS_WRITE);
	EXPECT_EQ(res, AMF_OK);
	res = pKernel->SetArgBuffer(1, input, AMF_ARGUMENT_ACCESS_READ);
	EXPECT_EQ(res, AMF_OK);
	res = pKernel->SetArgBuffer(2, input2, AMF_ARGUMENT_ACCESS_READ);
	EXPECT_EQ(res, AMF_OK);
	res = pKernel->SetArgInt32(3, 1024);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_Metal, 12_LaunchShader_Metal) {
	res = pKernel->GetCompileWorkgroupSize(sizeLocal);
	EXPECT_EQ(res, AMF_OK);
	res = pKernel->Enqueue(1, offset, sizeGlobal, NULL);
	EXPECT_EQ(res, AMF_OK);
	res = pCompute->FlushQueue();
	EXPECT_EQ(res, AMF_OK);
	res = pCompute->FinishQueue();
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_Metal, 13_MoveResultToHost_Metal) {
	output->MapToHost((void**)&outputData2, 0, 1024 * sizeof(float), true);
	EXPECT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_Metal, 14_CompareResultToExpected_Metal) {
	for (int k = 0; k < 1024; k++)
	{
		EXPECT_LE(abs(expectedData[k] - outputData2[k]), 0.01);
	}
}
#endif