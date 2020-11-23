#include "autotests.h"

struct AMF_Mutex : testing::Test {
	AMF_RESULT res;
	chrono::time_point<chrono::system_clock> startTime;
	thread threadObj;

	static void SetUpTestCase() {
		initiateTestSuiteLog("AMFMutex");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Mutex() {
		startTime = initiateTestLog();
	}

	~AMF_Mutex() {
		terminateTestLog(startTime);
	}
};

void printThread() {
	AMFMutex mutex1 = amf_open_mutex(L"\global1");
	mutex1.Lock();
	for (int i = 0; i < 10000; i++)
		std::cout << "thread function Executing" << std::endl;
	mutex1.Unlock();
}

TEST_F(AMF_Mutex, AMFMutex1) {
	AMFMutex mutex1 = amf_create_mutex(false, L"\global1");
	threadObj = thread(printThread);
	mutex1.Lock();
	for (int i = 0; i < 10000; i++)
		std::cout << "Display From MainThread" << std::endl;
	mutex1.Unlock();
	ASSERT_TRUE(false);
}

struct AMF_STL : testing::Test {
	AMF_RESULT res;
	chrono::time_point<chrono::system_clock> startTime;
	const amf_string str = "test";
	const amf_wstring wstr = L"test";
	amf_string res_str;
	amf_wstring res_wstr;

	static void SetUpTestCase() {
		initiateTestSuiteLog("AMF_STL");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_STL() {
		startTime = initiateTestLog();
	}

	~AMF_STL() {
		terminateTestLog(startTime);
	}
};

TEST_F(AMF_STL, amf_from_unicode_to_utf8) {
	res_str = amf_from_unicode_to_utf8(wstr);
	EXPECT_EQ(res_str, str);
}