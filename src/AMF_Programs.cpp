#include "Autotests.h"

static SharedVariables variables;

struct AMF_Programs : testing::Test {
	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_Programs");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Programs() {
		variables.startTime = initiateTestLog();
		variables.helper.Init();
		variables.factory = variables.helper.GetFactory();
		variables.factory->CreateContext(&variables.context1);
		variables.context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	}

	~AMF_Programs() {
		variables.context1.Release();
		terminateTestLog(variables.startTime);
	}
};

TEST_F(AMF_Programs, AMFPrograms_RegisterKernelSource) {
	AMFPrograms* program;
	variables.res = variables.factory->GetPrograms(&program);
	EXPECT_EQ(variables.res, AMF_OK);
	AMF_KERNEL_ID kernel = 0;
	const char* kernel_src = "\n" \
		"__kernel void square2( __global float* input, __global float* output, \n" \
		" const unsigned int count) {            \n" \
		" int i = get_global_id(0);              \n" \
		" if(i < count) \n" \
		" output[i] = input[i] * input[i]; \n" \
		"}                     \n";
	variables.res = program->RegisterKernelSource(&kernel, L"kernelIDName", "square2", strlen(kernel_src), (amf_uint8*)kernel_src, NULL);
	EXPECT_EQ(variables.res, AMF_OK);
	EXPECT_FALSE(kernel);
}

// TODO: Add test files and finish test
TEST_F(AMF_Programs, AMFPrograms_RegisterKernelSourceFile) {
	AMFPrograms* program;
	variables.res = variables.factory->GetPrograms(&program);
	EXPECT_EQ(variables.res, AMF_OK);
	AMF_KERNEL_ID kernel = 0;
	EXPECT_EQ(program->RegisterKernelSourceFile(&kernel, L"kenelIDname", "square2", L"test_shader.cl", NULL), AMF_OK);
}

// TODO: Add a coresponding binary and finish test
TEST_F(AMF_Programs, AMFPrograms_RegisterKernelBinary) {
	AMFPrograms* program;
	variables.res = variables.factory->GetPrograms(&program);
	EXPECT_EQ(variables.res, AMF_OK);
	AMF_KERNEL_ID kernel = 0;
	EXPECT_NE(program->RegisterKernelBinary(&kernel, L"kenelIDname", "square2", NULL, NULL, NULL), AMF_NOT_IMPLEMENTED);
}
