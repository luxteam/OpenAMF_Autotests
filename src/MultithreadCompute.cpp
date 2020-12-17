#include "Autotests.h"
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
	res = g_AMFFactory.Init();
	ASSERT_EQ(res, AMF_OK);
	res = helper.Init();
	ASSERT_EQ(res, AMF_OK);
	factory = helper.GetFactory();
}

TEST_F(MultithreadCompute, 2_CreateContextsMT) {
	res = factory->CreateContext(&context1);
	ASSERT_EQ(res, AMF_OK);
	res = context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	ASSERT_EQ(res, AMF_OK);
	res = context1->GetOpenCLComputeFactory(&oclComputeFactory);
	ASSERT_EQ(res, AMF_OK);
	res = factory->CreateContext(&context2);
	ASSERT_EQ(res, AMF_OK);
	res = context2->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	ASSERT_EQ(res, AMF_OK);
	res = context2->GetOpenCLComputeFactory(&oclComputeFactory);
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(MultithreadCompute, 3_CreateProgramsMT) {
	res = factory->GetPrograms(&pPrograms1);
	ASSERT_EQ(res, AMF_OK);
	res = pPrograms1->RegisterKernelSource(&kernel, L"kernelIDName", "plus2", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);
	ASSERT_EQ(res, AMF_OK);
	res = factory->GetPrograms(&pPrograms2);
	ASSERT_EQ(res, AMF_OK);
	res = pPrograms2->RegisterKernelSource(&kernel, L"kernelIDName", "square", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(MultithreadCompute, 4_InitializeDevicesMT) {
	res = oclComputeFactory->GetDeviceAt(0, &pComputeDevice);
	ASSERT_EQ(res, AMF_OK);
	pComputeDevice->GetNativeContext();
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(MultithreadCompute, 5_GetComputeFromDeviceMT) {
	res = pComputeDevice->CreateCompute(nullptr, &pCompute1);
	ASSERT_EQ(res, AMF_OK);
	res = pComputeDevice->CreateCompute(nullptr, &pCompute2);
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(MultithreadCompute, 6_LoadKernelIntoComputeMT) {
	res = pCompute1->GetKernel(kernel, &pKernel1);
	ASSERT_EQ(res, AMF_OK);
	res = pCompute2->GetKernel(kernel, &pKernel2);
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(MultithreadCompute, 7_InitOpenCLInContextWithComputeNativeCommandQueueMT) {
	res = context1->InitOpenCL(pCompute1->GetNativeCommandQueue());
	ASSERT_EQ(res, AMF_OK);
	res = context2->InitOpenCL(pCompute2->GetNativeCommandQueue());
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(MultithreadCompute, 8_AllocateBuffersMT) {
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &input);
	ASSERT_EQ(res, AMF_OK);
	res = context2->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &input2);
	ASSERT_EQ(res, AMF_OK);
	res = context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &output);
	ASSERT_EQ(res, AMF_OK);
	res = context2->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &output2);
	ASSERT_EQ(res, AMF_OK);
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
	res = input->Convert(AMF_MEMORY_OPENCL);
	ASSERT_EQ(res, AMF_OK);
	res = input2->Convert(AMF_MEMORY_OPENCL);
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(MultithreadCompute, 11_SetShaderArgumentsMT) {
	res = pKernel1->SetArgBuffer(0, output, AMF_ARGUMENT_ACCESS_WRITE);
	ASSERT_EQ(res, AMF_OK);
	res = pKernel1->SetArgBuffer(1, input, AMF_ARGUMENT_ACCESS_READ);
	ASSERT_EQ(res, AMF_OK);
	res = pKernel1->SetArgInt32(2, 1024);
	ASSERT_EQ(res, AMF_OK);

	res = pKernel2->SetArgBuffer(0, output2, AMF_ARGUMENT_ACCESS_WRITE);
	ASSERT_EQ(res, AMF_OK);
	res = pKernel2->SetArgBuffer(1, input2, AMF_ARGUMENT_ACCESS_READ);
	ASSERT_EQ(res, AMF_OK);
	res = pKernel2->SetArgInt32(2, 1024);
	ASSERT_EQ(res, AMF_OK);
}

void launchSecondKernel() {
	res = pKernel2->GetCompileWorkgroupSize(sizeLocal);
	ASSERT_EQ(res, AMF_OK);
	res = pKernel2->Enqueue(1, offset, sizeGlobal, NULL);
	ASSERT_EQ(res, AMF_OK);
	res = pCompute2->FlushQueue();
	ASSERT_EQ(res, AMF_OK);
	res = pCompute2->FinishQueue();
	ASSERT_EQ(res, AMF_OK);
	res = output->MapToHost((void**)&outputData2, 0, 1024 * sizeof(float), true);
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(MultithreadCompute, 12_LaunchShaderMT) {
	threadObj = thread(launchSecondKernel);
	res = pKernel1->GetCompileWorkgroupSize(sizeLocal);
	ASSERT_EQ(res, AMF_OK);
	res = pKernel1->Enqueue(1, offset, sizeGlobal, NULL);
	ASSERT_EQ(res, AMF_OK);
	res = pCompute1->FlushQueue();
	ASSERT_EQ(res, AMF_OK);
	res = pCompute1->FinishQueue();
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(MultithreadCompute, 13_MoveResultToHostMT) {
	res = output->MapToHost((void**)&outputData1, 0, 1024 * sizeof(float), true);
	ASSERT_EQ(res, AMF_OK);
	res = output2->MapToHost((void**)&outputData2, 0, 1024 * sizeof(float), true);
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(MultithreadCompute, 14_JoinThreadsMT) {
	threadObj.join();
}

TEST_F(MultithreadCompute, 15_CompareResultToExpectedMT) {
	for (int k = 0; k < 1024; k++)
	{
		EXPECT_LE(abs(expectedData[k] - outputData1[k]), 0.01);
	}
	for (int k = 0; k < 1024; k++)
	{
		EXPECT_LE(abs(expectedData2[k] - outputData2[k]), 0.01);
	}
}
