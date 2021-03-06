#include "Autotests.h"

static SharedVariables variables;

struct AMF_Compute_OpenCL : testing::Test {
	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_Compute_OpenCL");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Compute_OpenCL() {
		variables.startTime = initiateTestLog();
	}

	~AMF_Compute_OpenCL() {
		terminateTestLog(variables.startTime);
	}
};

TEST_F(AMF_Compute_OpenCL, 1_InitializateFactory_OCL) {
	variables.res = g_AMFFactory.Init();
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.helper.Init();
	ASSERT_EQ(variables.res, AMF_OK);
	variables.factory = variables.helper.GetFactory();
}

TEST_F(AMF_Compute_OpenCL, 2_CreateContext_OCL) {
	variables.res = variables.factory->CreateContext(&variables.context1);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->GetOpenCLComputeFactory(&variables.oclComputeFactory);
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 3_CreatePrograms_OCL) {
	variables.res = variables.factory->GetPrograms(&variables.pPrograms1);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pPrograms1->RegisterKernelSource(&variables.kernel, L"kernelIDName", "multiplication", strlen(variables.kernel_src), (amf_uint8*)variables.kernel_src, NULL);
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 4_InitializeDevice_OCL) {
	variables.res = variables.oclComputeFactory->GetDeviceAt(0, &variables.pComputeDevice);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.pComputeDevice->GetNativeContext();
}

TEST_F(AMF_Compute_OpenCL, 5_GetComputeFromDevice_OCL) {
	variables.res = variables.pComputeDevice->CreateCompute(nullptr, &variables.pCompute1);
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 6_LoadKernelIntoCompute_OCL) {
	variables.res = variables.pCompute1->GetKernel(variables.kernel, &variables.pKernel1);
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 7_InitOpenCLInContextWithComputeNativeCommandQueue_OCL) {
	variables.res = variables.context1->InitOpenCL(variables.pCompute1->GetNativeCommandQueue());
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 8_AllocateBuffers_OCL) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.input);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.input2);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &variables.output);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.inputData = static_cast<float*>(variables.input->GetNative());
	variables.inputData2 = static_cast<float*>(variables.input2->GetNative());
}

TEST_F(AMF_Compute_OpenCL, 9_InitializeRandomData_OCL) {
	for (int k = 0; k < 1024; k++)
	{
		variables.inputData[k] = rand() / 50.00;
		variables.inputData2[k] = rand() / 50.00;
		variables.expectedData[k] = variables.inputData[k] * variables.inputData2[k];
	}
}

TEST_F(AMF_Compute_OpenCL, 10_ConvertInputFromHostToOpenCL_OCL) {
	variables.res = variables.input->Convert(AMF_MEMORY_OPENCL);
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 11_SetShaderArguments_OCL) {
	variables.res = variables.pKernel1->SetArgBuffer(0, variables.output, AMF_ARGUMENT_ACCESS_WRITE);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pKernel1->SetArgBuffer(1, variables.input, AMF_ARGUMENT_ACCESS_READ);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pKernel1->SetArgBuffer(2, variables.input2, AMF_ARGUMENT_ACCESS_READ);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pKernel1->SetArgInt32(3, 1024);
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 12_LaunchShader_OCL) {
	variables.res = variables.pKernel1->GetCompileWorkgroupSize(variables.sizeLocal);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pKernel1->Enqueue(1, variables.offset, variables.sizeGlobal, NULL);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pCompute1->FlushQueue();
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pCompute1->FinishQueue();
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 13_MoveResultToHost_OCL) {
	variables.res = variables.output->MapToHost((void**)&variables.outputData2, 0, 1024 * sizeof(float), true);
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(AMF_Compute_OpenCL, 14_CompareResultToExpected_OCL) {
	for (int k = 0; k < 1024; k++)
	{
		ASSERT_LE(abs(variables.expectedData[k] - variables.outputData2[k]), 0.01);
	}
	delete variables.expectedData;
}
