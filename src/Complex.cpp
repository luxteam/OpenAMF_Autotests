#include "Autotests.h"
#include <thread>

struct Complex : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFComputeFactoryPtr oclComputeFactory;
	AMFFactory* factory;
	AMF_RESULT res;
	int deviceCount;
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("Complex");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	Complex() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		g_AMFFactory.Init();
		startTime = initiateTestLog();
	}

	~Complex() {
		oclComputeFactory.Release();
		g_AMFFactory.Terminate();
		helper.Terminate();
		terminateTestLog(startTime);
	}
};

TEST_F(Complex, kernel2_compute_complex_copenCl) {
	context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	context1->GetOpenCLComputeFactory(&oclComputeFactory);
	context1->InitOpenCL();
	g_AMFFactory.Init();
	deviceCount = oclComputeFactory->GetDeviceCount();
	g_AMFFactory.GetFactory()->SetCacheFolder(L"./cache");

	AMFPrograms* pPrograms;
	factory->GetPrograms(&pPrograms);

	AMF_KERNEL_ID kernel = 0;
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
	pPrograms->RegisterKernelSource(&kernel, L"kernelIDName", "multiplication", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);

	for (int i = 0; i < deviceCount; ++i)
	{
		AMF_RESULT res;
		AMFComputeDevicePtr pComputeDevice;
		oclComputeFactory->GetDeviceAt(i, &pComputeDevice);
		pComputeDevice->GetNativeContext();

		AMFComputePtr pCompute;
		pComputeDevice->CreateCompute(nullptr, &pCompute);

		AMFComputeKernelPtr pKernel;
		res = pCompute->GetKernel(kernel, &pKernel);

		AMFBuffer* input = NULL;
		AMFBuffer* input2 = NULL;
		AMFBuffer* output = NULL;

		AMFContextPtr context;
		factory->CreateContext(&context);
		context->InitOpenCL(pCompute->GetNativeCommandQueue());

		res = context->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &input);
		res = context->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &input2);
		res = context->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &output);

		float* inputData = static_cast<float*>(input->GetNative());
		float* inputData2 = static_cast<float*>(input2->GetNative());
		float* expectedData = new float[1024];
		for (int k = 0; k < 1024; k++)
		{
			inputData[k] = rand() / 50.00;
			inputData2[k] = rand() / 50.00;
			expectedData[k] = inputData[k] * inputData2[k];
		}

		input->Convert(AMF_MEMORY_OPENCL);

		res = pKernel->SetArgBuffer(0, output, AMF_ARGUMENT_ACCESS_WRITE);
		res = pKernel->SetArgBuffer(1, input, AMF_ARGUMENT_ACCESS_READ);
		res = pKernel->SetArgBuffer(2, input2, AMF_ARGUMENT_ACCESS_READ);
		res = pKernel->SetArgInt32(3, 1024);

		amf_size sizeLocal[3] = { 1024, 0, 0 };
		amf_size sizeGlobal[3] = { 1024, 0, 0 };
		amf_size offset[3] = { 0, 0, 0 };

		pKernel->GetCompileWorkgroupSize(sizeLocal);

		pKernel->Enqueue(1, offset, sizeGlobal, NULL);
		pCompute->FlushQueue();
		pCompute->FinishQueue();
		float* outputData2 = NULL;
		res = output->MapToHost((void**)&outputData2, 0, 1024 * sizeof(float), true);


		for (int k = 0; k < 1024; k++)
		{
			EXPECT_LE(abs(expectedData[k] - outputData2[k]), 0.01);
		}
	}
}

TEST_F(Complex, DISABLED_metal_compute_complex) {
	printf("start");
	const char* kernel_src = R"(
		#include <metal_stdlib>

		using namespace metal;

		kernel void process_array(
            device const float*     inA,
            device const float*     inB,
		    device float*           result,
            constant int32_t &      value,
            constant float &        extra,

            uint2 global_id [[thread_position_in_grid]],
            uint2 local_id [[thread_position_in_threadgroup]],
            uint2 group_id [[threadgroup_position_in_grid]],
            uint2 group_size [[threads_per_threadgroup]],
            uint2 grid_size [[threads_per_grid]]
            )
		{
            if(!global_id.x)
            {
                result[global_id.x] = global_id.y;
            }
            else if(1 == global_id.x)
            {
                result[global_id.x] = global_id.y;
            }

            else if(2 == global_id.x)
            {
                result[global_id.x] = local_id.x;
            }
            else if(3 == global_id.x)
            {
                result[global_id.x] = local_id.y;
            }

            else if(4 == global_id.x)
            {
                result[global_id.x] = local_id.x;
            }
            else if(5 == global_id.x)
            {
                result[global_id.x] = local_id.y;
            }

            else if(6 == global_id.x)
            {
                result[global_id.x] = group_id.x;
            }
            else if(7 == global_id.x)
            {
                result[global_id.x] = group_id.y;
            }

            else if(8 == global_id.x)
            {
                result[global_id.x] = group_size.x;
            }
            else if(9 == global_id.x)
            {
                result[global_id.x] = group_size.y;
            }

            else if(10 == global_id.x)
            {
                result[global_id.x] = grid_size.x;
            }
            else if(11 == global_id.x)
            {
                result[global_id.x] = grid_size.y;
            }

            else
            {
                result[global_id.x] = inA[global_id.x] + inB[global_id.x] - value + extra;
            }
		}
        )";
	AMFFactoryHelper helper;
	helper.Init();
	amf::AMFFactory* factory = helper.GetFactory();

	printf("0");
	amf::AMFContextPtr context;
	factory->CreateContext(&context);
	//context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	amf::AMFComputeFactoryPtr metalComputeFactory;
	context->GetMetalComputeFactory(&metalComputeFactory);
	int deviceCount = metalComputeFactory->GetDeviceCount();
	printf("\nDevice count :%d\n", deviceCount);

	amf::AMFComputeDevicePtr pComputeDevice;
	metalComputeFactory->GetDeviceAt(0, &pComputeDevice);

	context->InitMetalEx(pComputeDevice.GetPtr());

	amf::AMFPrograms* pPrograms;
	factory->GetPrograms(&pPrograms);

	amf::AMF_KERNEL_ID kernel = 0;
	pPrograms->RegisterKernelSource(&kernel, L"kernelIDName", "process_array", strlen(kernel_src), (amf_uint8*)kernel_src, "option");

	g_AMFFactory.Init();
	g_AMFFactory.GetDebug()->AssertsEnable(true);
	AMF_RESULT res;
	printf("1");
	//amf::AMFComputeDevicePtr pComputeDevice;

	amf::AMFComputePtr pCompute;
	//pComputeDevice->CreateCompute(nullptr, &pCompute);
	res = context->GetCompute(amf::AMF_MEMORY_METAL, &pCompute);
	printf("%d", (int)res);
	amf::AMFComputeKernelPtr pKernel;
	res = pCompute->GetKernel(kernel, &pKernel);

	amf::AMFBufferPtr input1;
	amf::AMFBufferPtr input2;
	amf::AMFBufferPtr output;

	const int arraysSize = 128;

	context->AllocBuffer(amf::AMF_MEMORY_HOST, arraysSize * sizeof(float), &input1);
	context->AllocBuffer(amf::AMF_MEMORY_HOST, arraysSize * sizeof(float), &input2);
	context->AllocBuffer(amf::AMF_MEMORY_METAL, arraysSize * sizeof(float), &output);

	{
		float  *inputData = static_cast<float*>(input1->GetNative());
		for (int k = 0; k < arraysSize; k++)
		{
			inputData[k] = k;
		}
	}

	{
		float  *inputData = static_cast<float*>(input2->GetNative());
		for (int k = 0; k < arraysSize; k++)
		{
			inputData[k] = arraysSize - k;
		}
	}

	input1->Convert(amf::AMF_MEMORY_METAL);
	input2->Convert(amf::AMF_MEMORY_METAL);

	pKernel->SetArgBuffer(0, input1, amf::AMF_ARGUMENT_ACCESS_READ);
	pKernel->SetArgBuffer(1, input2, amf::AMF_ARGUMENT_ACCESS_READ);
	pKernel->SetArgBuffer(2, output, amf::AMF_ARGUMENT_ACCESS_WRITE);
	pKernel->SetArgInt32(3, -1);
	pKernel->SetArgFloat(4, 0.345f);

	amf_size sizeLocal[3] = { arraysSize, 1, 1 };
	amf_size sizeGlobal[3] = { arraysSize, 1, 1 };
	amf_size offset[3] = { 0, 0, 0 };

	pKernel->GetCompileWorkgroupSize(sizeLocal);
	pKernel->Enqueue(1, offset, sizeGlobal, sizeLocal);
	pCompute->FlushQueue();

	//output->Convert(amf::AMF_MEMORY_HOST);
	amf::AMFBuffer *subBuffer = NULL;
	output->CreateSubBuffer(&subBuffer, 0, arraysSize * sizeof(float));
	float  pattern[4] = { 0.3, 0.4, 0.5, 0.6 };
	subBuffer->Fill(24 * sizeof(float), (arraysSize - 24) * sizeof(float), &pattern[0], 4 * sizeof(float));
	subBuffer->Convert(amf::AMF_MEMORY_HOST);

	float  *outputData = static_cast<float*>(subBuffer->GetNative());

	for (int k = 0; k < arraysSize; k++)
	{
		std::cout << "result[" << k << "] = " << outputData[k] << std::endl;
	}
}

TEST_F(Complex, DISABLED_openCL_complex_multithread) {
	const char* kernel_src = "\n" \
		"__kernel void cos_native_test( __global float* output, __global float* input, \n" \
		" const unsigned int count) {            \n" \
		" int i = get_global_id(0);              \n" \
		" if(i < count) \n" \
		" output[i] = input[i]; \n" \
		"}                     \n" \
		"__kernel void sin_half_test(__global float* output, __global float* input, __global float* input2, \n" \
		" const unsigned int count) {            \n" \
		" int i = get_global_id(0);              \n" \
		" if(i < count) \n" \
		" output[i] = input[i]; \n" \
		"}                     \n";

	AMFPrograms* programCPU;
	AMFPrograms* programGPU;
	AMF_KERNEL_ID kernel1 = 1;
	AMF_KERNEL_ID kernel2 = 2;
	factory->GetPrograms(&programCPU);
	factory->GetPrograms(&programGPU);
	programCPU->RegisterKernelSource(&kernel1, L"test_kernel_name", "cos_native_test", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);
	programGPU->RegisterKernelSource(&kernel2, L"test_kernel_name2", "sin_half_test", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);

	AMFComputeFactoryPtr computeFactoryCPU;
	AMFComputeFactoryPtr computeFactoryGPU;

	AMFContextPtr contextCPU;
	AMFContextPtr contextGPU;

	factory->CreateContext(&contextCPU);
	factory->CreateContext(&contextGPU);

	contextGPU->GetOpenCLComputeFactory(&computeFactoryGPU);
	contextGPU->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	contextGPU->InitOpenCL();

	contextCPU->GetOpenCLComputeFactory(&computeFactoryCPU);
	contextCPU->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_CPU);
	contextCPU->InitOpenCL();

	g_AMFFactory.Init();

	AMFComputeDevicePtr cpuDevice;
	AMFComputeDevicePtr gpuDevice;

	computeFactoryCPU->GetDeviceAt(0, &cpuDevice);
	computeFactoryGPU->GetDeviceAt(0, &gpuDevice);

	AMFComputePtr cpuCompute;
	AMFComputePtr gpuCompute;

	cpuDevice->CreateCompute(nullptr, &cpuCompute);
	gpuDevice->CreateCompute(nullptr, &gpuCompute);

	///////////////////////////////
	AMFBuffer* cpuInput = NULL;
	AMFBuffer* gpuInput = NULL;
	AMFBuffer* cpuOutput = NULL;
	AMFBuffer* gpuOutput = NULL;
	
	// Transfer data to CPU/GPU local memory
	contextCPU->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &cpuInput);
	contextCPU->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &cpuOutput);
	
	contextGPU->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &gpuInput);
	contextGPU->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &gpuOutput);

	float* inputDataCPU = static_cast<float*>(cpuInput->GetNative());
	float* inputDataGPU = static_cast<float*>(gpuInput->GetNative());
	float* expectedData = new float[1024];
	
	for (int k = 0; k < 1024; k++)
	{
		inputDataCPU[k] = rand() / 50.00;
		inputDataGPU[k] = rand() / 50.00;
		expectedData[k] = inputDataCPU[k] + inputDataGPU[k];
	}

	amf::AMFComputeKernelPtr pKernel1;
	cpuCompute->GetKernel(kernel1, &pKernel1);
	amf::AMFComputeKernelPtr pKernel2;
	gpuCompute->GetKernel(kernel2, &pKernel2);

	//////////////////////////////////
	cpuInput->Convert(AMF_MEMORY_OPENCL);
	gpuInput->Convert(AMF_MEMORY_OPENCL);

	pKernel1->SetArgBuffer(0, cpuOutput, AMF_ARGUMENT_ACCESS_WRITE);
	pKernel2->SetArgBuffer(0, gpuOutput, AMF_ARGUMENT_ACCESS_WRITE);
	pKernel1->SetArgBuffer(1, cpuInput, AMF_ARGUMENT_ACCESS_READ);
	pKernel2->SetArgBuffer(1, gpuInput, AMF_ARGUMENT_ACCESS_READ);
	pKernel1->SetArgInt32(3, 1024);
	pKernel2->SetArgInt32(3, 1024);

	amf_size sizeLocal[3] = { 1024, 0, 0 };
	amf_size sizeGlobal[3] = { 1024, 0, 0 };
	amf_size offset[3] = { 0, 0, 0 };

	pKernel1->GetCompileWorkgroupSize(sizeLocal);
	pKernel2->GetCompileWorkgroupSize(sizeLocal);

	pKernel1->Enqueue(1, offset, sizeGlobal, NULL);
	pKernel2->Enqueue(1, offset, sizeGlobal, NULL);

	cpuCompute->FlushQueue();
	gpuCompute->FlushQueue();

	cpuCompute->FinishQueue();
	gpuCompute->FinishQueue();

	float* outputDataCpu = NULL;
	float* outputDataGpu = NULL;
	cpuOutput->MapToHost((void**)&outputDataCpu, 0, 1024 * sizeof(float), true);
	gpuOutput->MapToHost((void**)&outputDataGpu, 0, 1024 * sizeof(float), true);


	for (int k = 0; k < 1024; k++)
	{
		EXPECT_LE(abs(expectedData[k] - outputDataCpu[k] - outputDataGpu[k]), 0.01);
	}
}
