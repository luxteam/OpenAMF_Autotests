#include "autotests.h"
#include <thread>

// Shared Variables //
static thread threadObj;
static AMFFactoryHelper helper;
static AMFContextPtr context1;
static AMFContextPtr context2;
static AMFComputeFactoryPtr oclComputeFactory;
static AMFFactory* factory;
static AMF_RESULT res;
static AMFPrograms* pPrograms1;
static AMFPrograms* pPrograms2;
static AMFComputeDevicePtr pComputeDevice;
static AMF_KERNEL_ID kernel = 0;
static AMFComputePtr pCompute1;
static AMFComputePtr pCompute2;
static AMFComputeKernelPtr pKernel1;
static AMFComputeKernelPtr pKernel2;
static AMFBuffer* input = NULL;
static AMFBuffer* input2 = NULL;
static AMFBuffer* output = NULL;
static AMFBuffer* output2 = NULL;
static float* inputData;
static float* inputData2;
static float* expectedData = new float[1024];
static float* expectedData2 = new float[1024];
static int deviceCount;
static amf_size sizeLocal[3] = { 1024, 0, 0 };
static amf_size sizeGlobal[3] = { 1024, 0, 0 };
static amf_size offset[3] = { 0, 0, 0 };
static float* outputData1 = NULL;
static float* outputData2 = NULL;
static const char* kernel_src = "\n" \
"__kernel void square( __global float* output, __global float* input, \n" \
" const unsigned int count) {            \n" \
" int i = get_global_id(0);              \n" \
" if(i < count) \n" \
" output[i] = input[i] * input[i]; \n" \
"}                     \n" \
"__kernel void plus2(__global float* output, __global float* input, \n" \
" const unsigned int count) {            \n" \
" int i = get_global_id(0);              \n" \
" if(i < count) \n" \
" output[i] = input[i] + 2.0; \n" \
"}                     \n";
static chrono::time_point<chrono::system_clock> startTime;

struct MultithreadCompute : testing::Test {
	static void SetUpTestCase() {
		initiateTestSuiteLog("MultithreadCompute");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	MultithreadCompute() {
		startTime = initiateTestLog();
	}

	~MultithreadCompute() {
		terminateTestLog(startTime);
	}
};

TEST_F(MultithreadCompute, 1_InitializateFactoryMT) {
	g_AMFFactory.Init();
	helper.Init();
	factory = helper.GetFactory();
}

TEST_F(MultithreadCompute, 2_CreateContextsMT) {
	factory->CreateContext(&context1);
	context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	context1->GetOpenCLComputeFactory(&oclComputeFactory);
	factory->CreateContext(&context2);
	context2->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	context2->GetOpenCLComputeFactory(&oclComputeFactory);
}

TEST_F(MultithreadCompute, 3_CreateProgramsMT) {
	factory->GetPrograms(&pPrograms1);
	pPrograms1->RegisterKernelSource(&kernel, L"kernelIDName", "plus2", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);
	factory->GetPrograms(&pPrograms2);
	pPrograms2->RegisterKernelSource(&kernel, L"kernelIDName", "square", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);
}

TEST_F(MultithreadCompute, 4_InitializeDevicesMT) {
	oclComputeFactory->GetDeviceAt(0, &pComputeDevice);
	pComputeDevice->GetNativeContext();
}

TEST_F(MultithreadCompute, 5_GetComputeFromDeviceMT) {
	pComputeDevice->CreateCompute(nullptr, &pCompute1);
	pComputeDevice->CreateCompute(nullptr, &pCompute2);
}

TEST_F(MultithreadCompute, 6_LoadKernelIntoComputeMT) {
	pCompute1->GetKernel(kernel, &pKernel1);
	pCompute2->GetKernel(kernel, &pKernel2);
}

TEST_F(MultithreadCompute, 7_InitOpenCLInContextWithComputeNativeCommandQueueMT) {
	context1->InitOpenCL(pCompute1->GetNativeCommandQueue());
	context2->InitOpenCL(pCompute2->GetNativeCommandQueue());
}

TEST_F(MultithreadCompute, 8_AllocateBuffersMT) {
	context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &input);
	context2->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &input2);
	context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &output);
	context2->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &output2);
	inputData = static_cast<float*>(input->GetNative());
	inputData2 = static_cast<float*>(input2->GetNative());
}

TEST_F(MultithreadCompute, 9_InitializeRandomDataMT) {
	for (int k = 0; k < 1024; k++)
	{
		inputData[k] = k;
		inputData2[k] = k;
		expectedData[k] = inputData[k] + 2.0;
		expectedData2[k] = inputData2[k] * inputData2[k];
	}
}

TEST_F(MultithreadCompute, 10_ConvertInputFromHostToOpenCLMT) {
	input->Convert(AMF_MEMORY_OPENCL);
	input2->Convert(AMF_MEMORY_OPENCL);
}

TEST_F(MultithreadCompute, 11_SetShaderArgumentsMT) {
	pKernel1->SetArgBuffer(0, output, AMF_ARGUMENT_ACCESS_WRITE);
	pKernel1->SetArgBuffer(1, input, AMF_ARGUMENT_ACCESS_READ);
	pKernel1->SetArgInt32(3, 1024);

	pKernel2->SetArgBuffer(0, output2, AMF_ARGUMENT_ACCESS_WRITE);
	pKernel2->SetArgBuffer(2, input2, AMF_ARGUMENT_ACCESS_READ);
	pKernel2->SetArgInt32(3, 1024);
}

void launchSecondKernel() {
	pKernel2->GetCompileWorkgroupSize(sizeLocal);
	pKernel2->Enqueue(1, offset, sizeGlobal, NULL);
	pCompute2->FlushQueue();
	pCompute2->FinishQueue();
	output->MapToHost((void**)&outputData2, 0, 1024 * sizeof(float), true);
}

TEST_F(MultithreadCompute, 12_LaunchShaderMT) {
	//threadObj = thread(launchSecondKernel);
	pKernel1->GetCompileWorkgroupSize(sizeLocal);
	pKernel1->Enqueue(1, offset, sizeGlobal, NULL);
	pCompute1->FlushQueue();
	pCompute1->FinishQueue();
}

TEST_F(MultithreadCompute, 13_MoveResultToHostMT) {
	output->MapToHost((void**)&outputData1, 0, 1024 * sizeof(float), true);
	output2->MapToHost((void**)&outputData2, 0, 1024 * sizeof(float), true);
}

TEST_F(MultithreadCompute, 14_JoinThreadsMT) {
	//threadObj.join();
}

TEST_F(MultithreadCompute, 15_CompareResultToExpectedMT) {
	for (int k = 0; k < 1024; k++)
	{
		EXPECT_LE(abs(expectedData[k] - outputData1[k]), 0.01);
	}
	//for (int k = 0; k < 1024; k++)
	//{
	//	EXPECT_LE(abs(expectedData2[k] - outputData2[k]), 0.01);
	//}
}