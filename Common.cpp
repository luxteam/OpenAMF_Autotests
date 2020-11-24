#include "autotests.h"

AMFMutex mutex1 = amf_create_mutex(false, L"\\global1");
amf_handle crit_section = amf_create_critical_section();
amf_handle event = amf_create_event(false, true, L"test_event");
amf_long x = 5;


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

/*
void printThread() {
	mutex1.Lock();
	for (int i = 0; i < 10; i++)
		std::cout << "thread function Executing" << std::endl;
	mutex1.Unlock();
}

TEST_F(AMF_Mutex, AMFMutex1) {
	threadObj = thread(printThread);
	mutex1.Lock();
	for (int i = 0; i < 10; i++)
		std::cout << "Display From MainThread" << std::endl;
	mutex1.Unlock();
	threadObj.join();
	ASSERT_TRUE(false);
}
*/
void inc() {
	amf_atomic_inc(&x);
}

TEST_F(AMF_Mutex, amf_atomic_inc) {
	threadObj = thread(inc);
	amf_atomic_inc(&x);
	threadObj.join();
	EXPECT_EQ(x, 7);
	x = 5;
}

TEST_F(AMF_Mutex, amf_atomic_dec) {
	threadObj = thread(inc);
	amf_atomic_dec(&x);
	threadObj.join();
	EXPECT_EQ(x, 5);
	x = 5;
}

#define MAX_ARRAY 5

int test_arr[MAX_ARRAY];

void EmptyArray()
{
	cout << "EmptyArray" << endl;
	amf_enter_critical_section(crit_section);
	for (int x = 0; x < (MAX_ARRAY + 1); x++) test_arr[x] = 0;
	Sleep(1000);
	amf_leave_critical_section(crit_section);
}

void PrintArray()
{
	cout << "PrintArray" << endl;
	amf_enter_critical_section(crit_section);
	for (int x = 0; x < (MAX_ARRAY + 1); x++) cout << test_arr[x] << " ";
	cout << endl;
	Sleep(1000);
	amf_leave_critical_section(crit_section);
}

void FullArray()
{
	cout << "FullArray" << endl;
	amf_enter_critical_section(crit_section);
	for (int x = 0; x < (MAX_ARRAY + 1); x++) test_arr[x] = x;
	Sleep(1000);
	amf_leave_critical_section(crit_section);
}

void operateArray() {
	EmptyArray();
	PrintArray();
	FullArray();
	PrintArray();
	EmptyArray();
	PrintArray();
}

TEST_F(AMF_Mutex, DISABLED_amf_create_critical_section) {
	threadObj = thread(operateArray);
	operateArray();
	EXPECT_TRUE(false);
}

TEST_F(AMF_Mutex, DISABLED_amf_delete_critical_section) {
	amf_delete_critical_section(crit_section);
	EXPECT_ANY_THROW(amf_enter_critical_section(crit_section));
	crit_section = amf_create_critical_section();
}

TEST_F(AMF_Mutex, DISABLED_amf_enter_critical_section) {
}

TEST_F(AMF_Mutex, DISABLED_amf_leave_critical_section) {
}

void eventTest() {
	amf_wait_for_event(event, 100);
	x *= 2;
}

TEST_F(AMF_Mutex, amf_create_event) {
	threadObj = thread(eventTest);
	x++;
	amf_set_event(event);
	threadObj.join();
	EXPECT_EQ(x, 12);
	x = 5;
}

TEST_F(AMF_Mutex, amf_delete_event) {
}

TEST_F(AMF_Mutex, amf_set_event) {
}

TEST_F(AMF_Mutex, amf_reset_event) {
}

TEST_F(AMF_Mutex, amf_wait_for_event) {
}

TEST_F(AMF_Mutex, amf_wait_for_event_timeout) {
}

TEST_F(AMF_Mutex, amf_create_mutex) {
}

TEST_F(AMF_Mutex, amf_open_mutex) {
}

TEST_F(AMF_Mutex, amf_delete_mutex) {
}

TEST_F(AMF_Mutex, amf_wait_for_mutex) {
}

TEST_F(AMF_Mutex, amf_release_mutex) {
}

TEST_F(AMF_Mutex, amf_create_semaphore) {
}

TEST_F(AMF_Mutex, amf_delete_semaphore) {
}

TEST_F(AMF_Mutex, amf_wait_for_semaphore) {
}

TEST_F(AMF_Mutex, amf_release_semaphore) {
}

TEST_F(AMF_Mutex, amf_sleep) {
}

TEST_F(AMF_Mutex, amf_high_precision_clock) {
}

TEST_F(AMF_Mutex, amf_increase_timer_precision) {
}

TEST_F(AMF_Mutex, amf_restore_timer_precision) {
}

TEST_F(AMF_Mutex, amf_load_library) {
}

TEST_F(AMF_Mutex, amf_get_proc_address) {
}

TEST_F(AMF_Mutex, amf_free_library) {
}

TEST_F(AMF_Mutex, get_current_thread_id) {
}

TEST_F(AMF_Mutex, amf_virtual_alloc) {
}

TEST_F(AMF_Mutex, amf_virtual_free) {
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

TEST_F(AMF_STL, amf_from_utf8_to_unicode) {
	res_wstr = amf_from_utf8_to_unicode(str);
	EXPECT_EQ(res_wstr, wstr);
}

TEST_F(AMF_STL, amf_from_unicode_to_multibyte) {
	res_str = amf_from_unicode_to_multibyte(wstr);
	EXPECT_EQ(res_str, str);
}

TEST_F(AMF_STL, amf_from_multibyte_to_unicode) {
	res_wstr = amf_from_multibyte_to_unicode(str);
	EXPECT_EQ(res_wstr, wstr);
}

TEST_F(AMF_STL, amf_from_string_to_hex_string) {
	res_str = amf_from_string_to_hex_string(str);
	EXPECT_EQ(res_str.c_str(), str.c_str());
}

TEST_F(AMF_STL, amf_from_hex_string_to_string) {
	res_str = amf_from_hex_string_to_string(str);
	EXPECT_EQ(res_str.c_str(), str.c_str());
}

TEST_F(AMF_STL, amf_string_to_lower) {
	const amf_string upper_str = "TEST";
	res_str = amf_string_to_lower(upper_str);
	EXPECT_EQ(res_str, str);
}

TEST_F(AMF_STL, amf_string_to_lower_wstr) {
	const amf_wstring upper_str = L"TEST";
	res_wstr = amf_string_to_lower(upper_str);
	EXPECT_EQ(res_wstr, wstr);
}

TEST_F(AMF_STL, amf_string_to_upper) {
	const amf_string upper_str = "TEST";
	res_str = amf_string_to_upper(str);
	EXPECT_EQ(res_str, upper_str);
}

TEST_F(AMF_STL, amf_string_to_upper_wstr) {
	const amf_wstring upper_str = L"TEST";
	res_wstr = amf_string_to_upper(wstr);
	EXPECT_EQ(res_wstr, upper_str);
}

TEST_F(AMF_STL, amf_from_unicode_to_url_utf8) {
	res_str = amf_from_unicode_to_url_utf8(wstr);
	EXPECT_EQ(res_str, str);
}

TEST_F(AMF_STL, amf_from_url_utf8_to_unicode) {
	res_wstr = amf_from_url_utf8_to_unicode(str);
	EXPECT_EQ(res_wstr, wstr);
}

TEST_F(AMF_STL, amf_convert_path_to_os_accepted_path) {
	res_wstr = amf_convert_path_to_os_accepted_path(wstr);
	EXPECT_EQ(res_wstr, wstr);
}

TEST_F(AMF_STL, amf_convert_path_to_url_accepted_path) {
	res_wstr = amf_convert_path_to_url_accepted_path(wstr);
	EXPECT_EQ(res_wstr, wstr);
}

TEST_F(AMF_STL, amf_string_format) {
	res_str = amf_string_format("test %s %d", "one", 1);
	EXPECT_EQ(res_str, "test one 1");
}

TEST_F(AMF_STL, amf_string_format_wstr) {
	res_wstr = amf_string_format(L"test %s %d", L"one", 1);
	EXPECT_EQ(res_wstr, L"test one 1");
}

TEST_F(AMF_STL, DISABLED_amf_string_formatVA) {
	//res_str = amf_string_formatVA(str.c_str);
	EXPECT_EQ(res_str, str);
}

TEST_F(AMF_STL, DISABLED_amf_string_formatVA_wstr) {
	res_str = amf_from_unicode_to_utf8(wstr);
	EXPECT_EQ(res_str, str);
}

TEST_F(AMF_STL, amf_string_ci_compare) {
	amf_wstring wstr2 = L"test";
	amf_int res = amf_string_ci_compare(wstr, wstr2);
	EXPECT_EQ(res, 0);
}

TEST_F(AMF_STL, amf_string_ci_rfind) {
	amf_wstring wstr2 = L"test";
	amf_int res = amf_string_ci_rfind(wstr, wstr2);
	EXPECT_EQ(res, 0);
}
