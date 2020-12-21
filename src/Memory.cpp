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

//--------------------AMFData--------------------

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

TEST_F(AMF_Memory, getExpectedMemoryTypeOpenCL){
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

//--------------------Buffers--------------------
//+++++AMFBuffer+++++

TEST_F(AMF_Memory, bufferSetSizeDontCrashWithCorrectSize) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	res = smartptr->SetSize(512 * sizeof(float));
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
}

TEST_F(AMF_Memory, bufferSetSizeDontCrashWithSizeMoreThanAllocated) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	res = smartptr->SetSize(2048 * sizeof(float));
	EXPECT_EQ(res, AMF_INVALID_ARG);
	EXPECT_NE(smartptr, nullptr);
}
//не кидает ошибку, при setSize больше аллокации

TEST_F(AMF_Memory, bufferSetSizeDontCrashWithIncorrectSize) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	res = smartptr->SetSize(-1);
	EXPECT_EQ(res, AMF_INVALID_ARG);
	EXPECT_NE(smartptr, nullptr);
}//аналогично, не кидает ошибку при некорректном вводе

TEST_F(AMF_Memory, bufferGetSizeWithCorrectSize) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_EQ(smartptr->GetSize(), 1024 * sizeof(float));
	res = smartptr->SetSize(512 * sizeof(float));
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_EQ(smartptr->GetSize(), 512 * sizeof(float));
}

TEST_F(AMF_Memory, bufferGetSizeWithSizeMoreThanAllocated) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	res = smartptr->SetSize(2048 * sizeof(float));
	EXPECT_EQ(res, AMF_INVALID_ARG);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_EQ(smartptr->GetSize(), 1024 * sizeof(float));
}

TEST_F(AMF_Memory, bufferGetSizeWithIncorrectSize) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	res = smartptr->SetSize(-1);
	EXPECT_EQ(res, AMF_INVALID_ARG);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_EQ(smartptr->GetSize(), 1024 * sizeof(float));
}

//Get a pointer to the AMFBuffer object’s data in host memory, mapping it to host memory when necessary.
TEST_F(AMF_Memory, bufferGetNativeWithMemoryHost) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	AMFBuffer* native = NULL;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_EQ(native, nullptr);
	EXPECT_NO_THROW(native = static_cast<AMFBuffer*>(smartptr->GetNative()));
	EXPECT_NE(native, nullptr);
}//как проверить ожидаемое значение?

TEST_F(AMF_Memory, DISABLED_bufferGetNativeWithNonMemoryHost) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	AMFBuffer* native = NULL;
	res = context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_EQ(native, nullptr);
	EXPECT_NO_THROW(native = static_cast<AMFBuffer*>(smartptr->GetNative()));
	EXPECT_NE(native, nullptr);
}

//+++++AMFBufferObserver+++++

TEST_F(AMF_Memory, DISABLED_bufferAddObserverThworsNothing) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	AMFBufferObserver* obs = NULL;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	EXPECT_NO_THROW(smartptr->AddObserver(obs));
}

TEST_F(AMF_Memory, DISABLED_bufferRemoveObserverThworsNothing) {
	AMF_RESULT res;
	AMFBufferPtr smartptr;
	AMFBufferObserver* obs = NULL;
	res = context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &smartptr);
	EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(smartptr, nullptr);
	smartptr->AddObserver(obs);
	EXPECT_NO_THROW(smartptr->RemoveObserver(obs));
}

/*TEST_F(AMF_Memory, bufferOnBufferDataRelease) {
* }
*/

//+++++AMFAudioBuffer+++++

TEST_F(AMF_Memory, audioBufferGetSize) {
	AMF_RESULT res;
	AMFAudioBufferPtr aubuf;
	res = context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &aubuf);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NE(aubuf, nullptr);
	* EXPECT_EQ(aubuf->GetSize(), 1024 * sizeof(amf_int32));
	*/
}

TEST_F(AMF_Memory, audioBufferGetNativeWithMemoryHost) {
	AMF_RESULT res;
	AMFAudioBufferPtr aubuf;
	AMFBuffer* native = NULL;
	res = context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &aubuf);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NE(aubuf, nullptr);
	* EXPECT_EQ(native, nullptr);
	* EXPECT_NO_THROW(native = static_cast<AMFBuffer*>(aubuf->GetNative()));
	* EXPECT_NE(native, nullptr);
	*/
}

TEST_F(AMF_Memory, audioBufferGetSampleCount) {
	AMF_RESULT res;
	AMFAudioBufferPtr aubuf;
	res = context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &aubuf);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NO_THROW(aubuf->GetSampleCount());
	*/
}

TEST_F(AMF_Memory, audioBufferGetSampleRate) {
	AMF_RESULT res;
	AMFAudioBufferPtr aubuf;
	res = context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &aubuf);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NO_THROW(aubuf->GetSampleRate());
	*/
}

TEST_F(AMF_Memory, audioBufferGetSampleSize) {
	AMF_RESULT res;
	AMFAudioBufferPtr aubuf;
	res = context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &aubuf);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NO_THROW(aubuf->GetSampleSize());
	*/
}

TEST_F(AMF_Memory, audioBufferGetChannelCount) {
	AMF_RESULT res;
	AMFAudioBufferPtr aubuf;
	res = context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &aubuf);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NO_THROW(aubuf->GetChannelCount());
	*/
}

TEST_F(AMF_Memory, audioBufferGetChannelLayout) {
	AMF_RESULT res;
	AMFAudioBufferPtr aubuf;
	res = context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &aubuf);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NO_THROW(aubuf->GetChannelLayout());
	*/
}

//+++++AMFAudioBufferObserver+++++

TEST_F(AMF_Memory, audioBufferAddObserverThworsNothing) {
	AMF_RESULT res;
	AMFAudioBufferPtr aubuf;
	AMFAudioBufferObserver* aobs = NULL;
	res = context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &aubuf);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(aubuf, nullptr);
	EXPECT_NO_THROW(aubuf->AddObserver(aobs));
	*/
}

TEST_F(AMF_Memory, audioBufferRemoveObserverThworsNothing) {
	AMF_RESULT res;
	AMFAudioBufferPtr aubuf;
	AMFAudioBufferObserver* aobs = NULL;
	res = context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &aubuf);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(aubuf, nullptr);
	aubuf->AddObserver(aobs);
	EXPECT_NO_THROW(aubuf->RemoveObserver(aobs));
	*/
}

/*TEST_F(AMF_Memory, audioBufferOnBufferDataRelease) {
* }
*/

//--------------------Surfaces--------------------
//+++++AMFSurface+++++

TEST_F(AMF_Memory, surfaceGetFormat) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(sur, nullptr);
	EXPECT_EQ(sur->GetFormat(), AMF_SURFACE_NV12);
	*/
}

TEST_F(AMF_Memory, surfaceGetPlanesCount) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(sur, nullptr);
	EXPECT_NO_THROW(sur->GetPlanesCount());
	*/
}

TEST_F(AMF_Memory, surfaceGetPlaneAt) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(sur, nullptr);
	EXPECT_NO_THROW(sur->GetPlaneAt(index));
	*/
}

TEST_F(AMF_Memory, surfaceGetPlane) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(sur, nullptr);
	EXPECT_NO_THROW(sur->GetPlane(type));
	*/
}

TEST_F(AMF_Memory, surfaceGetFrameType) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(sur, nullptr);
	EXPECT_NO_THROW(sur->GetFrameType());
	*/
}

TEST_F(AMF_Memory, surfaceSetFrameType) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(sur, nullptr);
	EXPECT_NO_THROW(sur->SetFrameType(type));
	*/
}

TEST_F(AMF_Memory, surfaceSetCrop) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(sur, nullptr);
	EXPECT_NO_THROW(sur->SetCrop(x, y, width, height));
	*/
}

//+++++AMFSurfaceObserver+++++

TEST_F(AMF_Memory, surfaceAddObserverThworsNothing) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	AMFSurfaceObserver* sobs = NULL;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(sur, nullptr);
	EXPECT_NO_THROW(sur->AddObserver(sobs));
	*/
}

TEST_F(AMF_Memory, surfaceRemoveObserverThworsNothing) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	AMFSurfaceObserver* sobs = NULL;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	EXPECT_NE(sur, nullptr);
	sur->AddObserver(sobs);
	EXPECT_NO_THROW(sur->RemoveObserver(sobs));
	*/
}

/*TEST_F(AMF_Memory,surfaceOnBufferDataRelease) {
* }
*/

//+++++AMFPlane+++++

TEST_F(AMF_Memory, planeGetType) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	AMFPlanePtr plane;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NE(sur, nullptr);
	* plane = sur->GetPlane(type);
	* EXPECT_EQ(plane->GetType(), type);
	*/
}

TEST_F(AMF_Memory, planeGetNative) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	AMFPlanePtr plane;
	AMFBuffer* native = NULL;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NE(sur, nullptr);
	* plane = sur->GetPlane(type);
	* EXPECT_NO_THROW(native = static_cast<AMFBuffer*>(plane->GetNative()));
	*/
}

TEST_F(AMF_Memory, planeGetPixelSizeLnBytes) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	AMFPlanePtr plane;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NE(sur, nullptr);
	* plane = sur->GetPlane(type);
	* EXPECT_NO_THROW(plane->GetPixelSizeLnBytes());
	*/
}

TEST_F(AMF_Memory, planeGetOffsetX) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	AMFPlanePtr plane;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NE(sur, nullptr);
	* plane = sur->GetPlane(type);
	* EXPECT_NO_THROW(plane->GetOffsetX());
	*/
}

TEST_F(AMF_Memory, planeGetOffsetY) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	AMFPlanePtr plane;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NE(sur, nullptr);
	* plane = sur->GetPlane(type);
	* EXPECT_NO_THROW(plane->GetOffsetY());
	*/
}

TEST_F(AMF_Memory, planeGetWidth) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	AMFPlanePtr plane;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NE(sur, nullptr);
	* plane = sur->GetPlane(type);
	* EXPECT_NO_THROW(plane->GetWidth());
	*/
}

TEST_F(AMF_Memory, planeGetHeight) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	AMFPlanePtr plane;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NE(sur, nullptr);
	* plane = sur->GetPlane(type);
	* EXPECT_NO_THROW(plane->GetHeight());
	*/
}

TEST_F(AMF_Memory, planeGetHPitch) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	AMFPlanePtr plane;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NE(sur, nullptr);
	* plane = sur->GetPlane(type);
	* EXPECT_NO_THROW(plane->GetHPitch());
	*/
}

TEST_F(AMF_Memory, planeGetVPitch) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	AMFPlanePtr plane;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NE(sur, nullptr);
	* plane = sur->GetPlane(type);
	* EXPECT_NO_THROW(plane->GetVPitch());
	*/
}

TEST_F(AMF_Memory, planeIsTiled) {
	AMF_RESULT res;
	AMFSurfacePtr sur;
	AMFPlanePtr plane;
	res = context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &sur);
	EXPECT_EQ(res, AMF_NOT_IMPLEMENTED);
	/*EXPECT_EQ(res, AMF_OK);
	* EXPECT_NE(sur, nullptr);
	* plane = sur->GetPlane(type);
	* EXPECT_NO_THROW(plane->IsTiled());
	*/
}