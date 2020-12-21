#include "Autotests.h"

struct AMF_Memory : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFFactory* factory = nullptr;
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_Memory");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Memory() {
		AMF_RESULT amr = helper.Init();
		if (amr != AMF_OK) {
			exit(1);
		}
		factory = helper.GetFactory();
		if (factory == nullptr) {
			exit(1);
		}
		amr = factory->CreateContext(&context1);
		if (amr != AMF_OK) {
			exit(1);
		}
		amr = context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		if (amr != AMF_OK) {
			exit(1);
		}
		startTime = initiateTestLog();
	}

	~AMF_Memory() {
		context1.Release();
		terminateTestLog(startTime);
	}
};


TEST_F(AMF_Memory, DISABLED_getDataTypeFromMemoryUnknown) {
	AMF_RESULT res;
	//AMFBuffer* buffer = nullptr; ==
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_UNKNOWN, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_ANY_THROW(smartptr->GetDataType());
}

TEST_F(AMF_Memory, getExpectedDataBufferType) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_EQ(smartptr->GetDataType(), AMF_DATA_BUFFER);
}

TEST_F(AMF_Memory, getExpectedDataSurfaceType) {
	AMF_RESULT res;
	AMFSurface* surface = nullptr;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &surface);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	//EXPECT_NE(surface, nullptr);
	//EXPECT_EQ(res, AMF_OK);
	//EXPECT_EQ(surface->GetDataType(), AMF_DATA_SURFACE);
}

TEST_F(AMF_Memory, getExpectedDataAudioBufferType) {
	AMF_RESULT res;
	AMFAudioBuffer* aubuf = nullptr;
	res = context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &aubuf);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	//EXPECT_NE(aubuf, nullptr);
	//EXPECT_EQ(res, AMF_OK);
	//EXPECT_EQ(aubuf->GetDataType(), AMF_DATA_AUDIO_BUFFER);
}

/*TEST_F(AMF_Memory, getExpectedDataUserType) {
*
	EXPECT_EQ(->GetDataType(), AMF_Memory_USER);
}*/

TEST_F(AMF_Memory, getExpectedMemoryHostType) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_EQ(smartptr->GetMemoryType(), AMF_MEMORY_HOST);
}

TEST_F(AMF_Memory, DISABLED_getExpectedMemoryTypeOpenCL){
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	//EXPECT_NE(smartptr, nullptr);
	//EXPECT_EQ(res, AMF_OK);
	//EXPECT_EQ(buffer->GetMemoryType(), AMF_MEMORY_OPENCL);
}


TEST_F(AMF_Memory, memoryHostBlockDuplicate) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	AMFBufferPtr smartptr_res;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_EQ(smartptr->GetMemoryType(), AMF_MEMORY_HOST);
	res = smartptr->Duplicate(AMF_MEMORY_HOST, (AMFData**)&smartptr_res);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr_res, nullptr);
	EXPECT_EQ(smartptr_res->GetDataType(), AMF_DATA_BUFFER);
	EXPECT_EQ(smartptr_res->GetMemoryType(), AMF_MEMORY_HOST);
}


TEST_F(AMF_Memory, DISABLED_convertMemoryHostToMemoryOpencl) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_EQ(smartptr->GetMemoryType(), AMF_MEMORY_HOST);
	res = smartptr->Convert(AMF_MEMORY_OPENCL);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_NE(smartptr->GetDataType(), AMF_MEMORY_OPENCL);
}

 TEST_F(AMF_Memory, interopMemoryHostToMemoryOpencl) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_EQ(smartptr->GetMemoryType(), AMF_MEMORY_HOST);
	res = smartptr->Interop(AMF_MEMORY_OPENCL);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_EQ(smartptr->GetDataType(), AMF_MEMORY_HOST);
}

TEST_F(AMF_Memory, checkDataObjectIsReusable) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_EQ(smartptr->IsReusable(), true);
}

TEST_F(AMF_Memory, DISABLED_checkDataObjectIsNotReusable) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	AMFData* data;
	res = context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	data = smartptr;
	EXPECT_NE(data, nullptr);
	EXPECT_EQ(data->IsReusable(), false);
}

TEST_F(AMF_Memory, DISABLED_checkDataObjectIsReusableOpenCL) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_EQ(smartptr->IsReusable(), true);
}

TEST_F(AMF_Memory, setPtsDontCrash) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_NO_THROW(smartptr->SetPts(3000));
}

TEST_F(AMF_Memory, getPtsDontCrash) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_NO_THROW(smartptr->GetPts());
}

TEST_F(AMF_Memory, getPtsWithSetPtsChecking) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_NO_THROW(smartptr->SetPts(3333));
	//smartptr->SetPts(3333);
	EXPECT_EQ(smartptr->GetPts(), 3333);
}

TEST_F(AMF_Memory, setDurationDontCrash) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_NO_THROW(smartptr->SetDuration(3000));
}

TEST_F(AMF_Memory, getDurationDontCrash) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_NO_THROW(smartptr->GetDuration());
}

TEST_F(AMF_Memory, getDurationWithSetDurationChecking) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_NO_THROW(smartptr->SetDuration(3333));
	//smartptr->SetDuration(3333);
	EXPECT_EQ(smartptr->GetDuration(), 3333);
}
