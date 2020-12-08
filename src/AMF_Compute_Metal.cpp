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
	g_AMFFactory.Init();
	helper.Init();
	factory = helper.GetFactory();
}

TEST_F(AMF_Compute_Metal, 2_CreateContext_Metal) {
	factory->CreateContext(&context);
	context->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	context->GetMetalComputeFactory(&metalComputeFactory);
}

TEST_F(AMF_Compute_Metal, 3_CreatePrograms_Metal) {
	factory->GetPrograms(&pPrograms);
	pPrograms->RegisterKernelSource(&kernel, L"kernelIDName", "multiplication", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);
}

TEST_F(AMF_Compute_Metal, 4_InitializeDevice_Metal) {
	metalComputeFactory->GetDeviceAt(0, &pComputeDevice);
	pComputeDevice->GetNativeContext();
}

TEST_F(AMF_Compute_Metal, 5_GetComputeFromDeviceAndLoadKernel_Metal) {
	pComputeDevice->CreateCompute(nullptr, &pCompute);
}

TEST_F(AMF_Compute_Metal, 6_LoadKernelIntoCompute_Metal) {
	pCompute->GetKernel(kernel, &pKernel);
}

TEST_F(AMF_Compute_Metal, 7_InitMetalInContext_Metal) {
	context->InitMetal();
}

TEST_F(AMF_Compute_Metal, 8_AllocateBuffers_Metal) {
	context->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &input);
	context->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &input2);
	context->AllocBuffer(AMF_MEMORY_METAL, 1024 * sizeof(float), &output);
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
	input->Convert(AMF_MEMORY_METAL);
}

TEST_F(AMF_Compute_Metal, 11_SetShaderArguments_Metal) {
	pKernel->SetArgBuffer(0, output, AMF_ARGUMENT_ACCESS_WRITE);
	pKernel->SetArgBuffer(1, input, AMF_ARGUMENT_ACCESS_READ);
	pKernel->SetArgBuffer(2, input2, AMF_ARGUMENT_ACCESS_READ);
	pKernel->SetArgInt32(3, 1024);
}

TEST_F(AMF_Compute_Metal, 12_LaunchShader_Metal) {
	pKernel->GetCompileWorkgroupSize(sizeLocal);
	pKernel->Enqueue(1, offset, sizeGlobal, NULL);
	pCompute->FlushQueue();
	pCompute->FinishQueue();
}

TEST_F(AMF_Compute_Metal, 13_MoveResultToHost_Metal) {
	output->MapToHost((void**)&outputData2, 0, 1024 * sizeof(float), true);
}

TEST_F(AMF_Compute_Metal, 14_CompareResultToExpected_Metal) {
	for (int k = 0; k < 1024; k++)
	{
		EXPECT_LE(abs(expectedData[k] - outputData2[k]), 0.01);
	}
}
#endif