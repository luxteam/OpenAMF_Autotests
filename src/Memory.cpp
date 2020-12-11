#include "Autotests.h"

struct AMF_Memory : testing::Test {
	AMFFactoryHelper helper;
	AMFContextPtr context1;
	AMFFactory* factory;
	AMF_RESULT res;
	chrono::time_point<chrono::system_clock> startTime;

	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_Memory");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Memory() {
		helper.Init();
		factory = helper.GetFactory();
		factory->CreateContext(&context1);
		context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		startTime = initiateTestLog();
	}

	~AMF_Memory() {
		context1.Release();
		terminateTestLog(startTime);
	}
};


TEST_F(AMF_Memory, DISABLED_getDataTypeFromMemoryUnknown) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_UNKNOWN, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_ANY_THROW(buffer->GetDataType());
}

TEST_F(AMF_Memory, getExpectedDataBufferType) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(buffer->GetDataType(), AMF_DATA_BUFFER);
}

TEST_F(AMF_Memory, getExpectedDataSurfaceType) {
	AMFSurface* surface;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &surface);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(surface->GetDataType(), AMF_DATA_SURFACE);
}

TEST_F(AMF_Memory, getExpectedDataAudioBufferType) {
	AMFAudioBuffer* aubuf;
	res = context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &aubuf);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(aubuf->GetDataType(), AMF_DATA_AUDIO_BUFFER);
}

/*TEST_F(AMF_Memory, getExpectedDataUserType) {
*
	EXPECT_EQ(->GetDataType(), AMF_Memory_USER);
}*/

TEST_F(AMF_Memory, getExpectedMemoryHostType) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(buffer->GetMemoryType(), AMF_MEMORY_HOST);
}

TEST_F(AMF_Memory, DISABLED_getExpectedMemoryTypeOpenCL){
	AMFBuffer* buffer;
	context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(buffer->GetMemoryType(), AMF_MEMORY_OPENCL);
}


TEST_F(AMF_Memory, memoryHostBlockDuplicate) {
	AMFBuffer* buffer;
	AMFBuffer* buffer_res;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	res = buffer->Duplicate(AMF_MEMORY_HOST, (AMFData**)&buffer_res);
	EXPECT_EQ(res, AMF_OK);
}


TEST_F(AMF_Memory, DISABLED_convertMemoryHostToMemoryOpencl) {
	AMFBuffer* buffer;
	AMFBuffer* buffer_res;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	res = buffer->Convert(AMF_MEMORY_OPENCL);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(buffer->GetDataType(), AMF_MEMORY_OPENCL);
}

TEST_F(AMF_Memory, interopMemoryHostToMemoryOpencl) {
	AMFBuffer* buffer;
	AMFBuffer* buffer_res;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	res = buffer->Interop(AMF_MEMORY_OPENCL);
	EXPECT_NE(res, AMF_NOT_IMPLEMENTED);
	EXPECT_EQ(buffer->GetDataType(), AMF_MEMORY_HOST);
}

TEST_F(AMF_Memory, checkDataObjectIsReusable) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(buffer->IsReusable(), true);
}

TEST_F(AMF_Memory, DISABLED_checkDataObjectIsNotReusable) {
	AMFBuffer* buffer;
	AMFData* data;
	context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(data->IsReusable(), false);
}

TEST_F(AMF_Memory, DISABLED_checkDataObjectIsReusableOpenCL) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_DX11, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_EQ(buffer->IsReusable(), false);
}

TEST_F(AMF_Memory, setPtsDontCrush) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NO_THROW(buffer->SetPts(3000));
}

TEST_F(AMF_Memory, getPtsDontCrush) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NO_THROW(buffer->GetPts());
}

TEST_F(AMF_Memory, getPtsWithSetPtsChecking) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	buffer->SetPts(3333);
	EXPECT_EQ(buffer->GetPts(), 3333);
}

TEST_F(AMF_Memory, setDurationDontCrush) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NO_THROW(buffer->SetDuration(3000));
}

TEST_F(AMF_Memory, getDurationDontCrush) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NO_THROW(buffer->GetDuration());
}

TEST_F(AMF_Memory, getDurationWithSetDurationChecking) {
	AMFBuffer* buffer;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &buffer);
	EXPECT_EQ(res, AMF_OK);
	buffer->SetDuration(3333);
	EXPECT_EQ(buffer->GetDuration(), 3333);
}
