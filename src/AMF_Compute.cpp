#include "autotests.h"

// Shared Variables //
AMFFactoryHelper helper;
AMFContextPtr context;
AMFComputeFactoryPtr oclComputeFactory;
AMFFactory* factory;
AMF_RESULT res;
AMFPrograms* pPrograms;
AMFComputeDevicePtr pComputeDevice;
AMF_KERNEL_ID kernel = 0;
AMFComputePtr pCompute;
AMFComputeKernelPtr pKernel;
AMFBuffer* input = NULL;
AMFBuffer* input2 = NULL;
AMFBuffer* output = NULL;
float* inputData;
float* inputData2;
float* expectedData = new float[1024];
int deviceCount;
amf_size sizeLocal[3] = { 1024, 0, 0 };
amf_size sizeGlobal[3] = { 1024, 0, 0 };
amf_size offset[3] = { 0, 0, 0 };
float* outputData2 = NULL;
const char* kernel_src = "\n" \
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
chrono::time_point<chrono::system_clock> startTime;

struct AMF_Compute : testing::Test {
	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_Compute");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Compute() {
		startTime = initiateTestLog();
	}

	~AMF_Compute() {
		terminateTestLog(startTime);
	}
};

TEST_F(AMF_Compute, 1_InitializateFactory) {
	g_AMFFactory.Init();
	helper.Init();
	factory = helper.GetFactory();
}

TEST_F(AMF_Compute, 2_CreateContext) {
	factory->CreateContext(&context);
	context->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	context->GetOpenCLComputeFactory(&oclComputeFactory);
}

TEST_F(AMF_Compute, 3_CreatePrograms) {
	factory->GetPrograms(&pPrograms);
	pPrograms->RegisterKernelSource(&kernel, L"kernelIDName", "multiplication", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);
}

TEST_F(AMF_Compute, 4_InitializeDevice) {
	oclComputeFactory->GetDeviceAt(0, &pComputeDevice);
	pComputeDevice->GetNativeContext();
}

TEST_F(AMF_Compute, 5_GetComputeFromDeviceAndLoadKernel) {
	pComputeDevice->CreateCompute(nullptr, &pCompute);
}

TEST_F(AMF_Compute, 6_LoadKernelIntoCompute) {
	pCompute->GetKernel(kernel, &pKernel);
}

TEST_F(AMF_Compute, 7_InitOpenCLInContextWithComputeNativeCommandQueue) {
	context->InitOpenCL(pCompute->GetNativeCommandQueue());
}

TEST_F(AMF_Compute, 8_AllocateBuffers) {
	context->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &input);
	context->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &input2);
	context->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &output);
	inputData = static_cast<float*>(input->GetNative());
	inputData2 = static_cast<float*>(input2->GetNative());
}

TEST_F(AMF_Compute, 9_InitializeRandomData) {
	for (int k = 0; k < 1024; k++)
	{
		inputData[k] = rand() / 50.00;
		inputData2[k] = rand() / 50.00;
		expectedData[k] = inputData[k] * inputData2[k];
	}
}

TEST_F(AMF_Compute, 10_ConvertInputFromHostToOpenCL) {
	input->Convert(AMF_MEMORY_OPENCL);
}

TEST_F(AMF_Compute, 11_SetShaderArguments) {
	pKernel->SetArgBuffer(0, output, AMF_ARGUMENT_ACCESS_WRITE);
	pKernel->SetArgBuffer(1, input, AMF_ARGUMENT_ACCESS_READ);
	pKernel->SetArgBuffer(2, input2, AMF_ARGUMENT_ACCESS_READ);
	pKernel->SetArgInt32(3, 1024);
}

TEST_F(AMF_Compute, 12_LaunchShader) {
	pKernel->GetCompileWorkgroupSize(sizeLocal);
	pKernel->Enqueue(1, offset, sizeGlobal, NULL);
	pCompute->FlushQueue();
	pCompute->FinishQueue();
}

TEST_F(AMF_Compute, 13_MoveResultToHost) {
	output->MapToHost((void**)&outputData2, 0, 1024 * sizeof(float), true);
}

TEST_F(AMF_Compute, 14_CompareResultToExpected) {
	for (int k = 0; k < 1024; k++)
	{
		EXPECT_LE(abs(expectedData[k] - outputData2[k]), 0.01);
	}
}

/*
TEST_F(AMF_Compute, kernel2_compute_AMF_Compute_copenCl) {
	for (int i = 0; i < deviceCount; ++i)
	{
		for (int k = 0; k < 1024; k++)
		{
			EXPECT_LE(abs(expectedData[k] - outputData2[k]), 0.01);
		}
	}
}
*/