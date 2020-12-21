#include "Autotests.h"

// Shared Variables //
static AMFFactoryHelper helper;
static AMFContextPtr context = NULL;

static AMFComputeFactoryPtr oclComputeFactory = NULL;
static AMFFactory* factory = NULL;

static AMF_RESULT res = AMF_OK;
static AMFPrograms* pPrograms = NULL;
static AMFComputeDevicePtr pComputeDevice = NULL;

static AMF_KERNEL_ID kernel = 0;
static AMFComputeKernelPtr pKernel = NULL;
static AMFComputePtr pCompute = NULL;
static const char* kernel_src = R"(
__kernel void square2( __global float* output, __global float* input,
 const unsigned int count) {
 int i = get_global_id(0);
 if(i < count)
 output[i] = input[i] * input[i];
}
__kernel void multiplication(__global float* output, __global float* input, __global float* input2,
 const unsigned int count) {
 int i = get_global_id(0);
 if(i < count)
 output[i] = input[i] * input2[i];
}
)";

static AMFBuffer* input = NULL;
static AMFBuffer* input2 = NULL;
static AMFBuffer* output = NULL;

static float* inputData = NULL;
static float* inputData2 = NULL;

static float* outputData2 = NULL;

static float* expectedData = new float[1024];
static int deviceCount = NULL;

static amf_size sizeLocal[3] = { 1024, 0, 0 };
static amf_size sizeGlobal[3] = { 1024, 0, 0 };
static amf_size offset[3] = { 0, 0, 0 };

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

TEST_F(AMF_Compute_OpenCL, 1_InitializateFactory_OCL) {
	res = g_AMFFactory.Init();
	ASSERT_EQ(res, AMF_OK);
	res = helper.Init();
	ASSERT_EQ(res, AMF_OK);
	factory = helper.GetFactory();
}

TEST_F(AMF_Compute_OpenCL, 2_CreateContext_OCL) {
	res = factory->CreateContext(&context);
	ASSERT_EQ(res, AMF_OK);
	res = context->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	ASSERT_EQ(res, AMF_OK);
	res = context->GetOpenCLComputeFactory(&oclComputeFactory);
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 3_CreatePrograms_OCL) {
	res = factory->GetPrograms(&pPrograms);
	ASSERT_EQ(res, AMF_OK);
	res = pPrograms->RegisterKernelSource(&kernel, L"kernelIDName", "multiplication", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 4_InitializeDevice_OCL) {
	res = oclComputeFactory->GetDeviceAt(0, &pComputeDevice);
	ASSERT_EQ(res, AMF_OK);
	pComputeDevice->GetNativeContext();
}

TEST_F(AMF_Compute_OpenCL, 5_GetComputeFromDevice_OCL) {
	res = pComputeDevice->CreateCompute(nullptr, &pCompute);
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 6_LoadKernelIntoCompute_OCL) {
	res = pCompute->GetKernel(kernel, &pKernel);
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 7_InitOpenCLInContextWithComputeNativeCommandQueue_OCL) {
	res = context->InitOpenCL(pCompute->GetNativeCommandQueue());
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 8_AllocateBuffers_OCL) {
	res = context->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &input);
	ASSERT_EQ(res, AMF_OK);
	res = context->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &input2);
	ASSERT_EQ(res, AMF_OK);
	res = context->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &output);
	ASSERT_EQ(res, AMF_OK);
	inputData = static_cast<float*>(input->GetNative());
	inputData2 = static_cast<float*>(input2->GetNative());
}

TEST_F(AMF_Compute_OpenCL, 9_InitializeRandomData_OCL) {
	for (int k = 0; k < 1024; k++)
	{
		inputData[k] = rand() / 50.00;
		inputData2[k] = rand() / 50.00;
		expectedData[k] = inputData[k] * inputData2[k];
	}
}

TEST_F(AMF_Compute_OpenCL, 10_ConvertInputFromHostToOpenCL_OCL) {
	res = input->Convert(AMF_MEMORY_OPENCL);
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 11_SetShaderArguments_OCL) {
	res = pKernel->SetArgBuffer(0, output, AMF_ARGUMENT_ACCESS_WRITE);
	ASSERT_EQ(res, AMF_OK);
	res = pKernel->SetArgBuffer(1, input, AMF_ARGUMENT_ACCESS_READ);
	ASSERT_EQ(res, AMF_OK);
	res = pKernel->SetArgBuffer(2, input2, AMF_ARGUMENT_ACCESS_READ);
	ASSERT_EQ(res, AMF_OK);
	res = pKernel->SetArgInt32(3, 1024);
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 12_LaunchShader_OCL) {
	res = pKernel->GetCompileWorkgroupSize(sizeLocal);
	ASSERT_EQ(res, AMF_OK);
	res = pKernel->Enqueue(1, offset, sizeGlobal, NULL);
	ASSERT_EQ(res, AMF_OK);
	res = pCompute->FlushQueue();
	ASSERT_EQ(res, AMF_OK);
	res = pCompute->FinishQueue();
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 13_MoveResultToHost_OCL) {
	res = output->MapToHost((void**)&outputData2, 0, 1024 * sizeof(float), true);
	ASSERT_EQ(res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 14_CompareResultToExpected_OCL) {
	for (int k = 0; k < 1024; k++)
	{
		ASSERT_LE(abs(expectedData[k] - outputData2[k]), 0.01);
	}
	delete expectedData;
}
