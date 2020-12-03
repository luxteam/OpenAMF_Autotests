#include "autotests.h"

// Shared Variables //
static AMFFactoryHelper helper;
static AMFContextPtr context;
static AMFComputeFactoryPtr oclComputeFactory;
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

struct AMF_Compute_OpenCL : testing::Test {
	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_Compute_OpenCL");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Compute_OpenCL() {
		startTime = initiateTestLog();
	}

	~AMF_Compute_OpenCL() {
		terminateTestLog(startTime);
	}
};

TEST_F(AMF_Compute_OpenCL, 1_InitializateFactory) {
	g_AMFFactory.Init();
	helper.Init();
	factory = helper.GetFactory();
}

TEST_F(AMF_Compute_OpenCL, 2_CreateContext) {
	factory->CreateContext(&context);
	context->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	context->GetOpenCLComputeFactory(&oclComputeFactory);
}

TEST_F(AMF_Compute_OpenCL, 3_CreatePrograms) {
	factory->GetPrograms(&pPrograms);
	pPrograms->RegisterKernelSource(&kernel, L"kernelIDName", "multiplication", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);
}

TEST_F(AMF_Compute_OpenCL, 4_InitializeDevice) {
	oclComputeFactory->GetDeviceAt(0, &pComputeDevice);
	pComputeDevice->GetNativeContext();
}

TEST_F(AMF_Compute_OpenCL, 5_GetComputeFromDevice) {
	pComputeDevice->CreateCompute(nullptr, &pCompute);
}

TEST_F(AMF_Compute_OpenCL, 6_LoadKernelIntoCompute) {
	pCompute->GetKernel(kernel, &pKernel);
}

TEST_F(AMF_Compute_OpenCL, 7_InitOpenCLInContextWithComputeNativeCommandQueue) {
	context->InitOpenCL(pCompute->GetNativeCommandQueue());
}

TEST_F(AMF_Compute_OpenCL, 8_AllocateBuffers) {
	context->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &input);
	context->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &input2);
	context->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &output);
	inputData = static_cast<float*>(input->GetNative());
	inputData2 = static_cast<float*>(input2->GetNative());
}

TEST_F(AMF_Compute_OpenCL, 9_InitializeRandomData) {
	for (int k = 0; k < 1024; k++)
	{
		inputData[k] = rand() / 50.00;
		inputData2[k] = rand() / 50.00;
		expectedData[k] = inputData[k] * inputData2[k];
	}
}

TEST_F(AMF_Compute_OpenCL, 10_ConvertInputFromHostToOpenCL) {
	input->Convert(AMF_MEMORY_OPENCL);
}

TEST_F(AMF_Compute_OpenCL, 11_SetShaderArguments) {
	pKernel->SetArgBuffer(0, output, AMF_ARGUMENT_ACCESS_WRITE);
	pKernel->SetArgBuffer(1, input, AMF_ARGUMENT_ACCESS_READ);
	pKernel->SetArgBuffer(2, input2, AMF_ARGUMENT_ACCESS_READ);
	pKernel->SetArgInt32(3, 1024);
}

TEST_F(AMF_Compute_OpenCL, 12_LaunchShader) {
	pKernel->GetCompileWorkgroupSize(sizeLocal);
	pKernel->Enqueue(1, offset, sizeGlobal, NULL);
	pCompute->FlushQueue();
	pCompute->FinishQueue();
}

TEST_F(AMF_Compute_OpenCL, 13_MoveResultToHost) {
	output->MapToHost((void**)&outputData2, 0, 1024 * sizeof(float), true);
}

TEST_F(AMF_Compute_OpenCL, 14_CompareResultToExpected) {
	for (int k = 0; k < 1024; k++)
	{
		EXPECT_LE(abs(expectedData[k] - outputData2[k]), 0.01);
	}
}