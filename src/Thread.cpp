#include "autotests.h"
#include <thread>

static AMFMutex mutex1 = amf_create_mutex(false, L"\\global1");
static amf_handle mutex2 = amf_create_mutex(false, L"\\global2");
static amf_handle crit_section = amf_create_critical_section();
static amf_handle event = amf_create_event(false, true, L"test_event");
static amf_long x = 5;


struct AMF_Thread : testing::Test {
	AMF_RESULT res;
	chrono::time_point<chrono::system_clock> startTime;
	thread threadObj;

	static void SetUpTestCase() {
		initiateTestSuiteLog("AMFMutex");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	AMF_Thread() {
		startTime = initiateTestLog();
	}

	~AMF_Thread() {
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

TEST_F(AMF_Thread, AMFMutex1) {
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

TEST_F(AMF_Thread, amf_atomic_inc) {
	threadObj = thread(inc);
	amf_atomic_inc(&x);
	threadObj.join();
	EXPECT_EQ(x, 7);
	x = 5;
}

TEST_F(AMF_Thread, amf_atomic_dec) {
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

TEST_F(AMF_Thread, DISABLED_amf_enter_critical_section) {
	threadObj = thread(operateArray);
	operateArray();
	EXPECT_TRUE(false);
}

TEST_F(AMF_Thread, DISABLED_amf_delete_critical_section) {
	amf_delete_critical_section(crit_section);
	EXPECT_ANY_THROW(amf_enter_critical_section(crit_section));
	crit_section = amf_create_critical_section();
}

void eventTest() {
	amf_wait_for_event(event, 100);
	x *= 2;
}

TEST_F(AMF_Thread, amf_set_event) {
	threadObj = thread(eventTest);
	x++;
	amf_set_event(event);
	threadObj.join();
	EXPECT_EQ(x, 12);
	x = 5;
}

TEST_F(AMF_Thread, amf_delete_event) {
	amf_delete_event(event);
}

TEST_F(AMF_Thread, amf_reset_event) {
	EXPECT_NO_THROW(amf_reset_event(event));
}

TEST_F(AMF_Thread, amf_wait_for_event_timeout) {
	EXPECT_NO_THROW(amf_wait_for_event_timeout(event, 10));
}

TEST_F(AMF_Thread, DISABLED_amf_open_mutex) {
	threadObj = thread(inc);
	AMFMutex mutex = amf_open_mutex(L"\\global1");
	mutex.Lock();
	x *= 3;
	mutex.Unlock();
	EXPECT_TRUE(x == 18 || x == 16);
	x = 5;
}

TEST_F(AMF_Thread, amf_delete_mutex) {
	amf_delete_mutex(mutex2);
	amf_wait_for_mutex(mutex2, 100);
}

TEST_F(AMF_Thread, amf_release_mutex) {
	amf_wait_for_mutex(mutex2, 100);
	amf_release_mutex(mutex2);
}
/*
TEST_F(AMF_Thread, amf_create_semaphore) {
}

TEST_F(AMF_Thread, amf_delete_semaphore) {
}

TEST_F(AMF_Thread, amf_wait_for_semaphore) {
}

TEST_F(AMF_Thread, amf_release_semaphore) {
}

TEST_F(AMF_Thread, amf_sleep) {
}

TEST_F(AMF_Thread, amf_high_precision_clock) {
}

TEST_F(AMF_Thread, amf_increase_timer_precision) {
}

TEST_F(AMF_Thread, amf_restore_timer_precision) {
}

TEST_F(AMF_Thread, amf_load_library) {
}

TEST_F(AMF_Thread, amf_get_proc_address) {
}

TEST_F(AMF_Thread, amf_free_library) {
}

TEST_F(AMF_Thread, get_current_thread_id) {
}

TEST_F(AMF_Thread, amf_virtual_alloc) {
}

TEST_F(AMF_Thread, amf_virtual_free) {
}
*/