#include "Autotests.h"

static SharedVariables variables;

struct MultithreadCompute : testing::Test {
	static void SetUpTestCase() {
		initiateTestSuiteLog("MultithreadCompute");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	MultithreadCompute() {
		variables.startTime = initiateTestLog();
	}

	~MultithreadCompute() {
		terminateTestLog(variables.startTime);
	}
};

TEST_F(MultithreadCompute, 1_InitializateFactoryMT) {
	variables.res = g_AMFFactory.Init();
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.helper.Init();
	ASSERT_EQ(variables.res, AMF_OK);
	variables.factory = variables.helper.GetFactory();
}

TEST_F(MultithreadCompute, 2_CreateContextsMT) {
	variables.res = variables.factory->CreateContext(&variables.context1);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->GetOpenCLComputeFactory(&variables.oclComputeFactory);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.factory->CreateContext(&variables.context2);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.context2->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.context2->GetOpenCLComputeFactory(&variables.oclComputeFactory);
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(MultithreadCompute, 3_CreateProgramsMT) {
	variables.res = variables.factory->GetPrograms(&variables.pPrograms1);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pPrograms1->RegisterKernelSource(&variables.kernel, L"kernelIDName", "plus2", strlen(variables.kernel_src), (amf_uint8*)variables.kernel_src, NULL);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.factory->GetPrograms(&variables.pPrograms2);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pPrograms2->RegisterKernelSource(&variables.kernel, L"kernelIDName", "square", strlen(variables.kernel_src), (amf_uint8*)variables.kernel_src, NULL);
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(MultithreadCompute, 4_InitializeDevicesMT) {
	variables.res = variables.oclComputeFactory->GetDeviceAt(0, &variables.pComputeDevice);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.pComputeDevice->GetNativeContext();
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(MultithreadCompute, 5_GetComputeFromDeviceMT) {
	variables.res = variables.pComputeDevice->CreateCompute(nullptr, &variables.pCompute1);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pComputeDevice->CreateCompute(nullptr, &variables.pCompute2);
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(MultithreadCompute, 6_LoadKernelIntoComputeMT) {
	variables.res = variables.pCompute1->GetKernel(variables.kernel, &variables.pKernel1);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pCompute2->GetKernel(variables.kernel, &variables.pKernel2);
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(MultithreadCompute, 7_InitOpenCLInContextWithComputeNativeCommandQueueMT) {
	variables.res = variables.context1->InitOpenCL(variables.pCompute1->GetNativeCommandQueue());
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.context2->InitOpenCL(variables.pCompute2->GetNativeCommandQueue());
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(MultithreadCompute, 8_AllocateBuffersMT) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.input);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.context2->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.input2);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &variables.output);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.context2->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &variables.output2);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.inputData = static_cast<float*>(variables.input->GetNative());
	variables.inputData2 = static_cast<float*>(variables.input2->GetNative());
}

TEST_F(MultithreadCompute, 9_InitializeRandomDataMT) {
	for (int k = 0; k < 1024; k++)
	{
		variables.inputData[k] = k;
		variables.inputData2[k] = k;
		variables.expectedData[k] = variables.inputData[k] + 2.0;
		variables.expectedData2[k] = variables.inputData2[k] * variables.inputData2[k];
	}
}

TEST_F(MultithreadCompute, 10_ConvertInputFromHostToOpenCLMT) {
	variables.res = variables.input->Convert(AMF_MEMORY_OPENCL);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.input2->Convert(AMF_MEMORY_OPENCL);
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(MultithreadCompute, 11_SetShaderArgumentsMT) {
	variables.res = variables.pKernel1->SetArgBuffer(0, variables.output, AMF_ARGUMENT_ACCESS_WRITE);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pKernel1->SetArgBuffer(1, variables.input, AMF_ARGUMENT_ACCESS_READ);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pKernel1->SetArgInt32(2, 1024);
	ASSERT_EQ(variables.res, AMF_OK);

	variables.res = variables.pKernel2->SetArgBuffer(0, variables.output2, AMF_ARGUMENT_ACCESS_WRITE);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pKernel2->SetArgBuffer(1, variables.input2, AMF_ARGUMENT_ACCESS_READ);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pKernel2->SetArgInt32(2, 1024);
	ASSERT_EQ(variables.res, AMF_OK);
}

void launchSecondKernel() {
	variables.res = variables.pKernel2->GetCompileWorkgroupSize(variables.sizeLocal);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pKernel2->Enqueue(1, variables.offset, variables.sizeGlobal, NULL);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pCompute2->FlushQueue();
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pCompute2->FinishQueue();
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.output->MapToHost((void**)&variables.outputData2, 0, 1024 * sizeof(float), true);
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(MultithreadCompute, 12_LaunchShaderMT) {
	variables.threadObj = thread(launchSecondKernel);
	variables.res = variables.pKernel1->GetCompileWorkgroupSize(variables.sizeLocal);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pKernel1->Enqueue(1, variables.offset, variables.sizeGlobal, NULL);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pCompute1->FlushQueue();
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pCompute1->FinishQueue();
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(MultithreadCompute, 13_MoveResultToHostMT) {
	variables.res = variables.output->MapToHost((void**)&variables.outputData1, 0, 1024 * sizeof(float), true);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.output2->MapToHost((void**)&variables.outputData2, 0, 1024 * sizeof(float), true);
	ASSERT_EQ(variables.res, AMF_OK);
}

TEST_F(MultithreadCompute, 14_JoinThreadsMT) {
	variables.threadObj.join();
}

TEST_F(MultithreadCompute, 15_CompareResultToExpectedMT) {
	for (int k = 0; k < 1024; k++)
	{
		EXPECT_LE(abs(variables.expectedData[k] - variables.outputData1[k]), 0.01);
	}
	for (int k = 0; k < 1024; k++)
	{
		EXPECT_LE(abs(variables.expectedData2[k] - variables.outputData2[k]), 0.01);
	}
}
