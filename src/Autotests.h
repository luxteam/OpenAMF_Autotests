#pragma once
#ifndef H_UTILITY_AUTOTESTS
#define H_UTILITY_AUTOTESTS
#include "../../../common/Thread.h"
#include "../../../common/AMFSTL.h"
#include "../../../include/core/Factory.h"
#include "../../../common/AMFFactory.h"
#include "../../../include/core/Buffer.h"
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>  
#include <cmath>
#include <thread>
using namespace std;
using namespace amf;

#define CL_TARGET_OPENCL_VERSION 120

struct TestsInformation;

struct AllocationMetrics {
	uint32_t totalAllocated = 0;
	uint32_t totalFreed = 0;
	uint32_t totalPointersMade = 0;
	uint32_t totalPointersDestroyed = 0;

	uint32_t CurrentUsage();

	uint32_t CurrentPointers();
};

struct SharedVariables {
	thread threadObj;
	AMFFactoryHelper helper;
	AMFContextPtr context1 = NULL;
	AMFContextPtr context2 = NULL;
	AMFComputeFactoryPtr oclComputeFactory = NULL;
	AMFComputeFactoryPtr metalComputeFactory = NULL;
	AMFFactory* factory = NULL;
	AMF_RESULT res = AMF_NOT_SUPPORTED;
	AMFPrograms* pPrograms1 = NULL;
	AMFPrograms* pPrograms2 = NULL;
	AMFComputeDevicePtr pComputeDevice = NULL;
	AMF_KERNEL_ID kernel = 0;
	AMFComputePtr pCompute1 = NULL;
	AMFComputePtr pCompute2 = NULL;
	AMFComputeKernelPtr pKernel1 = NULL;
	AMFComputeKernelPtr pKernel2 = NULL;
	AMFBuffer* input = NULL;
	AMFBuffer* input2 = NULL;
	AMFBuffer* output = NULL;
	AMFBuffer* output2 = NULL;
	AMFBufferPtr smartptr;
	AMFSurfacePtr surface;
	AMFAudioBufferPtr aubuf;
	AMFPlanePtr plane;
	float* inputData = NULL;
	float* inputData2 = NULL;
	float* expectedData = new float[1024];
	float* expectedData2 = new float[1024];
	int deviceCount = NULL;
	amf_size sizeLocal[3] = { 1024, 0, 0 };
	amf_size sizeGlobal[3] = { 1024, 0, 0 };
	amf_size offset[3] = { 0, 0, 0 };
	float* outputData1 = NULL;
	float* outputData2 = NULL;
	const char* kernel_src = R"(
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
	const char* kernel_src_metal = R"(
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
	const char* kernel_src_multithread = R"(
	__kernel void square( __global float* output, __global float* input, 
	const unsigned int count) {            
	int i = get_global_id(0);              
	if(i < count) 
	output[i] = input[i] * input[i]; 
	}                     
	__kernel void plus2(__global float* output, __global float* input, 
	const unsigned int count) {            
	int i = get_global_id(0);              
	if(i < count) 
	output[i] = input[i] + 2.0; 
	}
	)";
	chrono::time_point<chrono::system_clock> startTime;
};

void* operator new(size_t size);

void operator delete(void* memory, size_t size);

bool has_suffix(const string& str, const string& suffix);

void initiateTestSuiteLog(string suiteName);

chrono::time_point<chrono::system_clock> initiateTestLog();

void terminateTestLog(chrono::time_point<chrono::system_clock> startTime);

void terminateTestSuiteLog();

int allocationHook(int allocType, void* userData, std::size_t size, int blockType, long requestNumber,
	const unsigned char* filename, int lineNumber);

#endif
