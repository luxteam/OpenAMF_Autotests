#include "Autotests.h"

static SharedVariables variables;

struct AMF_Memory : testing::Test {

	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_Memory");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Memory() {
		AMF_RESULT amr = variables.helper.Init();
		assert(amr == AMF_OK);

		variables.factory = variables.helper.GetFactory();
		assert(variables.factory != nullptr);

		amr = variables.factory->CreateContext(&variables.context1);
		assert(amr == AMF_OK);

		amr = variables.context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
		assert(amr == AMF_OK);

		variables.startTime = initiateTestLog();
	}

	~AMF_Memory() {
		variables.context1.Release();
		terminateTestLog(variables.startTime);
	}
};

static void initOpenCL() {
	variables.res = g_AMFFactory.Init();
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.helper.Init();
	ASSERT_EQ(variables.res, AMF_OK);
	variables.factory = variables.helper.GetFactory();
	ASSERT_NE(variables.factory, nullptr);
	variables.res = variables.factory->CreateContext(&variables.context1);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->SetProperty(AMF_CONTEXT_DEVICE_TYPE, AMF_CONTEXT_DEVICE_TYPE_GPU);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->GetOpenCLComputeFactory(&variables.oclComputeFactory);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.factory->GetPrograms(&variables.pPrograms1);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pPrograms1->RegisterKernelSource(&variables.kernel, L"kernelIDName", "multiplication", strlen(variables.kernel_src), (amf_uint8*)variables.kernel_src, NULL);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.oclComputeFactory->GetDeviceAt(0, &variables.pComputeDevice);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.pComputeDevice->GetNativeContext();
	ASSERT_NE(variables.pComputeDevice, nullptr);
	variables.res = variables.pComputeDevice->CreateCompute(nullptr, &variables.pCompute1);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.pCompute1->GetKernel(variables.kernel, &variables.pKernel1);
	ASSERT_EQ(variables.res, AMF_OK);
	variables.res = variables.context1->InitOpenCL(variables.pCompute1->GetNativeCommandQueue());
	ASSERT_EQ(variables.res, AMF_OK);
}

//--------------------AMFData--------------------

TEST_F(AMF_Memory, DISABLED_getDataTypeFromMemoryUnknown) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_UNKNOWN, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_ANY_THROW(variables.smartptr->GetDataType());
	//краш теста, вместо корректной обработки
}

TEST_F(AMF_Memory, getExpectedDataBufferType) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_EQ(variables.smartptr->GetDataType(), AMF_DATA_BUFFER);
}

TEST_F(AMF_Memory, getExpectedDataSurfaceType) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_NE(variables.surface, nullptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_EQ(variables.surface->GetDataType(), AMF_DATA_SURFACE);
	*/
}

TEST_F(AMF_Memory, getExpectedDataAudioBufferType) {
	variables.res = variables.context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &variables.aubuf);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_NE(variables.aubuf, nullptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_EQ(variables.aubuf->GetDataType(), AMF_DATA_AUDIO_BUFFER);
	*/
}

/*TEST_F(AMF_Memory, getExpectedDataUserType) {
*
	ASSERT_EQ(->GetDataType(), AMF_Memory_USER);
	//баг документации - непонятно, как вообще взаимодействовать с типом AMF_Memory_USER
}*/

TEST_F(AMF_Memory, getExpectedMemoryHostType) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_EQ(variables.smartptr->GetMemoryType(), AMF_MEMORY_HOST);
}
//todo
TEST_F(AMF_Memory, getExpectedMemoryTypeOpenCL){
	initOpenCL();
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_EQ(variables.smartptr->GetMemoryType(), AMF_MEMORY_OPENCL);
}


TEST_F(AMF_Memory, memoryHostBlockDuplicate) {
	AMFBufferPtr smartptr_res;
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_EQ(variables.smartptr->GetMemoryType(), AMF_MEMORY_HOST);
	variables.res = variables.smartptr->Duplicate(AMF_MEMORY_HOST, (AMFData**)&smartptr_res);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(smartptr_res, nullptr);
	ASSERT_EQ(smartptr_res->GetDataType(), AMF_DATA_BUFFER);
	ASSERT_EQ(smartptr_res->GetMemoryType(), AMF_MEMORY_HOST);
}


TEST_F(AMF_Memory, convertMemoryHostToMemoryOpenCL) {
	initOpenCL();
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_EQ(variables.smartptr->GetMemoryType(), AMF_MEMORY_HOST);
	variables.res = variables.smartptr->Convert(AMF_MEMORY_OPENCL);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_EQ(variables.smartptr->GetDataType(), AMF_MEMORY_OPENCL);
} //кидает amf_ok, но после конвертации тип не изменяется

 TEST_F(AMF_Memory, interopMemoryHostToMemoryOpenCL) {
	initOpenCL();
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_EQ(variables.smartptr->GetMemoryType(), AMF_MEMORY_HOST);
	variables.res = variables.smartptr->Interop(AMF_MEMORY_OPENCL);
	ASSERT_EQ(variables.res, AMF_NOT_SUPPORTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_EQ(variables.smartptr->GetDataType(), AMF_MEMORY_HOST);*/
}

TEST_F(AMF_Memory, checkDataObjectIsReusable) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_EQ(variables.smartptr->IsReusable(), true);
}

TEST_F(AMF_Memory, checkDataObjectIsNotReusable) {
	initOpenCL();
	AMFData* data;
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	data = variables.smartptr;
	ASSERT_NE(data, nullptr);
	ASSERT_EQ(data->IsReusable(), false);
}//в целом непонятно на что сейчас кидается false

TEST_F(AMF_Memory, checkDataObjectIsReusableOpenCL) {
	initOpenCL();
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_EQ(variables.smartptr->IsReusable(), true);
}

TEST_F(AMF_Memory, setPtsDontCrash) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_NO_THROW(variables.smartptr->SetPts(3000));
}

TEST_F(AMF_Memory, getPtsDontCrash) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_NO_THROW(variables.smartptr->GetPts());
}

TEST_F(AMF_Memory, getPtsWithSetPtsChecking) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_NO_THROW(variables.smartptr->SetPts(3333));
	//variables.smartptr->SetPts(3333);
	ASSERT_EQ(variables.smartptr->GetPts(), 3333);
}

TEST_F(AMF_Memory, setDurationDontCrash) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_NO_THROW(variables.smartptr->SetDuration(3000));
}

TEST_F(AMF_Memory, getDurationDontCrash) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_NO_THROW(variables.smartptr->GetDuration());
}

TEST_F(AMF_Memory, getDurationWithSetDurationChecking) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_NO_THROW(variables.smartptr->SetDuration(3333));
	//variables.smartptr->SetDuration(3333);
	ASSERT_EQ(variables.smartptr->GetDuration(), 3333);
}

//--------------------Buffers--------------------
//+++++AMFBuffer+++++

TEST_F(AMF_Memory, bufferSetSizeDontCrashWithCorrectSize) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	variables.res = variables.smartptr->SetSize(512 * sizeof(float));
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
}

TEST_F(AMF_Memory, bufferSetSizeDontCrashWithSizeMoreThanAllocated) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	variables.res = variables.smartptr->SetSize(2048 * sizeof(float));
	ASSERT_EQ(variables.res, AMF_INVALID_ARG);
	ASSERT_NE(variables.smartptr, nullptr);
} //не кидает ошибку, при setSize больше аллокации

TEST_F(AMF_Memory, bufferSetSizeDontCrashWithIncorrectSize) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	variables.res = variables.smartptr->SetSize(-1);
	ASSERT_EQ(variables.res, AMF_INVALID_ARG);
	ASSERT_NE(variables.smartptr, nullptr);
} //не кидает ошибку при некорректном вводе

TEST_F(AMF_Memory, bufferGetSizeWithCorrectSize) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_EQ(variables.smartptr->GetSize(), 1024 * sizeof(float));
	variables.res = variables.smartptr->SetSize(512 * sizeof(float));
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_EQ(variables.smartptr->GetSize(), 512 * sizeof(float));
}

TEST_F(AMF_Memory, bufferGetSizeWithSizeMoreThanAllocated) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	variables.res = variables.smartptr->SetSize(2048 * sizeof(float));
	ASSERT_EQ(variables.res, AMF_INVALID_ARG);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_EQ(variables.smartptr->GetSize(), 1024 * sizeof(float));
} //не кидает ошибку при некорректном вводе

TEST_F(AMF_Memory, bufferGetSizeWithIncorrectSize) {
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	variables.res = variables.smartptr->SetSize(-1);
	ASSERT_EQ(variables.res, AMF_INVALID_ARG);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_EQ(variables.smartptr->GetSize(), 1024 * sizeof(float));
} //не кидает ошибку при некорректном вводе

TEST_F(AMF_Memory, bufferGetNativeWithMemoryHost) {
	AMFBuffer* native = NULL;
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_EQ(native, nullptr);
	ASSERT_NO_THROW(native = static_cast<AMFBuffer*>(variables.smartptr->GetNative()));
	ASSERT_NE(native, nullptr);
}

TEST_F(AMF_Memory, bufferGetNativeWithNonMemoryHost) {
	initOpenCL();
	AMFBuffer* native = NULL;
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_OPENCL, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_EQ(native, nullptr);
	ASSERT_NO_THROW(native = static_cast<AMFBuffer*>(variables.smartptr->GetNative()));
	ASSERT_NE(native, nullptr);
}

//+++++AMFBufferObserver+++++

TEST_F(AMF_Memory, DISABLED_bufferAddObserverThworsNothing) {
	AMFBufferObserver* obs = NULL;
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	ASSERT_NO_THROW(variables.smartptr->AddObserver(obs));
} //непонятно, как проверять работу с обсерверами

TEST_F(AMF_Memory, DISABLED_bufferRemoveObserverThworsNothing) {
	AMFBufferObserver* obs = NULL;
	variables.res = variables.context1->AllocBuffer(AMF_MEMORY_HOST, 1024 * sizeof(float), &variables.smartptr);
	ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.smartptr, nullptr);
	variables.smartptr->AddObserver(obs);
	ASSERT_NO_THROW(variables.smartptr->RemoveObserver(obs));
} //непонятно, как проверять работу с обсерверами

/*TEST_F(AMF_Memory, bufferOnBufferDataRelease) {
* }
*/

//+++++AMFAudioBuffer+++++

TEST_F(AMF_Memory, audioBufferGetSize) {
	variables.res = variables.context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &variables.aubuf);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NE(variables.aubuf, nullptr);
	* ASSERT_EQ(variables.aubuf->GetSize(), 1024 * sizeof(amf_int32));
	*/
}

TEST_F(AMF_Memory, audioBufferGetNativeWithMemoryHost) {
	AMFBuffer* native = NULL;
	variables.res = variables.context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &variables.aubuf);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NE(variables.aubuf, nullptr);
	* ASSERT_EQ(native, nullptr);
	* ASSERT_NO_THROW(native = static_cast<AMFBuffer*>(variables.aubuf->GetNative()));
	* ASSERT_NE(native, nullptr);
	*/
}

TEST_F(AMF_Memory, audioBufferGetSampleCount) {
	variables.res = variables.context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &variables.aubuf);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NO_THROW(variables.aubuf->GetSampleCount());
	*/
}

TEST_F(AMF_Memory, audioBufferGetSampleRate) {
	variables.res = variables.context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &variables.aubuf);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NO_THROW(variables.aubuf->GetSampleRate());
	*/
}

TEST_F(AMF_Memory, audioBufferGetSampleSize) {
	variables.res = variables.context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &variables.aubuf);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NO_THROW(variables.aubuf->GetSampleSize());
	*/
}

TEST_F(AMF_Memory, audioBufferGetChannelCount) {
	variables.res = variables.context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &variables.aubuf);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NO_THROW(variables.aubuf->GetChannelCount());
	*/
}

TEST_F(AMF_Memory, audioBufferGetChannelLayout) { 
	variables.res = variables.context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &variables.aubuf);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NO_THROW(variables.aubuf->GetChannelLayout());
	*/
}

//+++++AMFAudioBufferObserver+++++

TEST_F(AMF_Memory, audioBufferAddObserverThworsNothing) {
	AMFAudioBufferObserver* aobs = NULL;
	variables.res = variables.context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &variables.aubuf);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.aubuf, nullptr);
	ASSERT_NO_THROW(variables.aubuf->AddObserver(aobs));
	*/
}

TEST_F(AMF_Memory, audioBufferRemoveObserverThworsNothing) {
	AMFAudioBufferObserver* aobs = NULL;
	variables.res = variables.context1->AllocAudioBuffer(AMF_MEMORY_HOST, AMFAF_S32, 1024 * sizeof(amf_int32), 1024, 16, &variables.aubuf);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.aubuf, nullptr);
	variables.aubuf->AddObserver(aobs);
	ASSERT_NO_THROW(variables.aubuf->RemoveObserver(aobs));
	*/
}

/*TEST_F(AMF_Memory, audioBufferOnBufferDataRelease) {
* }
*/

//--------------------Surfaces--------------------
//+++++AMFSurface+++++

TEST_F(AMF_Memory, surfaceGetFormat) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.surface, nullptr);
	ASSERT_EQ(variables.surface->GetFormat(), AMF_SURFACE_NV12);
	*/
}

TEST_F(AMF_Memory, surfaceGetPlanesCount) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.surface, nullptr);
	ASSERT_NO_THROW(variables.surface->GetPlanesCount());
	*/
}

TEST_F(AMF_Memory, surfaceGetPlaneAt) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.surface, nullptr);
	ASSERT_NO_THROW(variables.surface->GetPlaneAt(index));
	*/
}

TEST_F(AMF_Memory, surfaceGetPlane) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.surface, nullptr);
	ASSERT_NO_THROW(variables.surface->GetPlane(type));
	*/
}

TEST_F(AMF_Memory, surfaceGetFrameType) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.surface, nullptr);
	ASSERT_NO_THROW(variables.surface->GetFrameType());
	*/
}

TEST_F(AMF_Memory, surfaceSetFrameType) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.surface, nullptr);
	ASSERT_NO_THROW(variables.surface->SetFrameType(type));
	*/
}

TEST_F(AMF_Memory, surfaceSetCrop) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.surface, nullptr);
	ASSERT_NO_THROW(variables.surface->SetCrop(x, y, width, height));
	*/
}

//+++++AMFSurfaceObserver+++++

TEST_F(AMF_Memory, surfaceAddObserverThworsNothing) {
	AMFSurfaceObserver* sobs = NULL;
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.surface, nullptr);
	ASSERT_NO_THROW(variables.surface->AddObserver(sobs));
	*/
}

TEST_F(AMF_Memory, surfaceRemoveObserverThworsNothing) {
	AMFSurfaceObserver* sobs = NULL;
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	ASSERT_NE(variables.surface, nullptr);
	variables.surface->AddObserver(sobs);
	ASSERT_NO_THROW(variables.surface->RemoveObserver(sobs));
	*/
}

/*TEST_F(AMF_Memory,surfaceOnBufferDataRelease) {
* }
*/

//+++++AMFPlane+++++

TEST_F(AMF_Memory, planeGetType) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NE(variables.surface, nullptr);
	* variables.plane = variables.surface->GetPlane(type);
	* ASSERT_EQ(variables.plane->GetType(), type);
	*/
}

TEST_F(AMF_Memory, planeGetNative) {
	AMFBuffer* native = NULL;
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NE(variables.surface, nullptr);
	* variables.plane = variables.surface->GetPlane(type);
	* ASSERT_NO_THROW(native = static_cast<AMFBuffer*>(variables.plane->GetNative()));
	*/
}

TEST_F(AMF_Memory, planeGetPixelSizeLnBytes) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NE(variables.surface, nullptr);
	* variables.plane = variables.surface->GetPlane(type);
	* ASSERT_NO_THROW(variables.plane->GetPixelSizeLnBytes());
	*/
}

TEST_F(AMF_Memory, planeGetOffsetX) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NE(variables.surface, nullptr);
	* variables.plane = variables.surface->GetPlane(type);
	* ASSERT_NO_THROW(variables.plane->GetOffsetX());
	*/
}

TEST_F(AMF_Memory, planeGetOffsetY) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NE(variables.surface, nullptr);
	* variables.plane = variables.surface->GetPlane(type);
	* ASSERT_NO_THROW(variables.plane->GetOffsetY());
	*/
}

TEST_F(AMF_Memory, planeGetWidth) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NE(variables.surface, nullptr);
	* variables.plane = variables.surface->GetPlane(type);
	* ASSERT_NO_THROW(variables.plane->GetWidth());
	*/
}

TEST_F(AMF_Memory, planeGetHeight) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NE(variables.surface, nullptr);
	* variables.plane = variables.surface->GetPlane(type);
	* ASSERT_NO_THROW(variables.plane->GetHeight());
	*/
}

TEST_F(AMF_Memory, planeGetHPitch) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NE(variables.surface, nullptr);
	* variables.plane = variables.surface->GetPlane(type);
	* ASSERT_NO_THROW(variables.plane->GetHPitch());
	*/
}

TEST_F(AMF_Memory, planeGetVPitch) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NE(variables.surface, nullptr);
	* variables.plane = variables.surface->GetPlane(type);
	* ASSERT_NO_THROW(variables.plane->GetVPitch());
	*/
}

TEST_F(AMF_Memory, planeIsTiled) {
	variables.res = variables.context1->AllocSurface(AMF_MEMORY_DX9, AMF_SURFACE_NV12, 1024 * sizeof(float), 1024 * sizeof(float), &variables.surface);
	ASSERT_EQ(variables.res, AMF_NOT_IMPLEMENTED);
	/*ASSERT_EQ(variables.res, AMF_OK);
	* ASSERT_NE(variables.surface, nullptr);
	* variables.plane = variables.surface->GetPlane(type);
	* ASSERT_NO_THROW(variables.plane->IsTiled());
	*/
}