#include "Autotests.h"
#include <thread>

static AMFMutex mutex1 = amf_create_mutex(false, L"/test1");
static amf_handle mutex2 = amf_create_mutex(false, L"/test2");
static amf_handle crit_section = amf_create_critical_section();
static amf_handle semaphore = amf_create_semaphore(0, 2, L"/test3");
static amf_handle event = amf_create_event(false, true, L"test_event");
static amf_long x = 5;
static bool res_bool = true;


struct Thread : testing::Test {
	AMF_RESULT res;
	chrono::time_point<chrono::system_clock> startTime;
	thread threadObj;

	static void SetUpTestCase() {
		initiateTestSuiteLog("AMFMutex");
	}

	static void TearDownTestCase() {
		terminateTestSuiteLog();
	}

	void checkResources() {
		ASSERT_NE(mutex2, (amf_handle)NULL);
		ASSERT_NE(crit_section, (amf_handle)NULL);
		ASSERT_NE(semaphore, (amf_handle)NULL);
		ASSERT_NE(event, (amf_handle)NULL);
	}

	Thread() {
		startTime = initiateTestLog();
	}

	~Thread() {
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

TEST_F(Thread, AMFMutex1) {
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
	res_bool = amf_atomic_inc(&x);
	ASSERT_EQ(res_bool, true);
}

TEST_F(Thread, amf_atomic_inc) {
	threadObj = thread(inc);
	res_bool = amf_atomic_inc(&x);
	ASSERT_EQ(res_bool, true);
	threadObj.join();
	EXPECT_EQ(x, 7);
	x = 5;
}

TEST_F(Thread, amf_atomic_dec) {
	threadObj = thread(inc);
	res_bool = amf_atomic_dec(&x);
	ASSERT_EQ(res_bool, true);
	threadObj.join();
	EXPECT_EQ(x, 5);
	x = 5;
}

#define MAX_ARRAY 5

int test_arr[MAX_ARRAY];

void EmptyArray()
{
	cout << "EmptyArray" << endl;
	res_bool = amf_enter_critical_section(crit_section);
	ASSERT_EQ(res_bool, true);
	for (int x = 0; x < (MAX_ARRAY + 1); x++) test_arr[x] = 0;
	res_bool = amf_leave_critical_section(crit_section);
	ASSERT_EQ(res_bool, true);
}

void PrintArray()
{
	cout << "PrintArray" << endl;
	res_bool = amf_enter_critical_section(crit_section);
	ASSERT_EQ(res_bool, true);
	for (int x = 0; x < (MAX_ARRAY + 1); x++) cout << test_arr[x] << " ";
	cout << endl;
	res_bool = amf_leave_critical_section(crit_section);
	ASSERT_EQ(res_bool, true);
}

void FullArray()
{
	cout << "FullArray" << endl;
	res_bool = amf_enter_critical_section(crit_section);
	ASSERT_EQ(res_bool, true);
	for (int x = 0; x < (MAX_ARRAY + 1); x++) test_arr[x] = x;
	res_bool = amf_leave_critical_section(crit_section);
	ASSERT_EQ(res_bool, true);
}

void operateArray() {
	EmptyArray();
	PrintArray();
	FullArray();
	PrintArray();
	EmptyArray();
	PrintArray();
}

TEST_F(Thread, DISABLED_amf_enter_critical_section) {
	threadObj = thread(operateArray);
	operateArray();
	EXPECT_TRUE(false);
}

TEST_F(Thread, DISABLED_amf_delete_critical_section) {
	res_bool = amf_delete_critical_section(crit_section);
	ASSERT_EQ(res_bool, true);
	EXPECT_ANY_THROW(amf_enter_critical_section(crit_section));
	crit_section = amf_create_critical_section();
}

void eventTest() {
	res_bool = amf_wait_for_event(event, 100);
	ASSERT_EQ(res_bool, true);
	x *= 2;
}

void printThread2() {
	for (int i = 0; i < 100; i++)
		std::cout << "thread function Executing" << std::endl;
}

int i = 0;

void makeACallFromPhoneBooth()
{
	res_bool = amf_wait_for_mutex(mutex2, 20);
	ASSERT_EQ(res_bool, true);
	res_bool = amf_wait_for_mutex(mutex2, 20);
	ASSERT_EQ(res_bool, true);
	std::cout << i << " Hello Wife" << std::endl;
	i++;
	res_bool = amf_release_mutex(mutex2);
	ASSERT_EQ(res_bool, true);
}

TEST_F(Thread, recursive_mutex) {
	std::thread man1(makeACallFromPhoneBooth);
	std::thread man2(makeACallFromPhoneBooth);
	std::thread man3(makeACallFromPhoneBooth);

	man1.join();
	man2.join();
	man3.join();
	EXPECT_FALSE(true);
}

TEST_F(Thread, amf_set_event) {
	threadObj = thread(eventTest);
	x++;
	res_bool = amf_set_event(event);
	ASSERT_EQ(res_bool, true);
	threadObj.join();
	EXPECT_EQ(x, 12);
	x = 5;
}

TEST_F(Thread, amf_delete_event) {
	res_bool = amf_delete_event(event);
	ASSERT_EQ(res_bool, true);
}

TEST_F(Thread, amf_reset_event) {
	EXPECT_NO_THROW(amf_reset_event(event));
}

TEST_F(Thread, amf_wait_for_event_timeout) {
	EXPECT_NO_THROW(amf_wait_for_event_timeout(event, 10));
}

TEST_F(Thread, DISABLED_amf_open_mutex) {
	threadObj = thread(inc);
	AMFMutex mutex = amf_open_mutex(L"\\global1");
	res_bool = mutex.Lock();
	ASSERT_EQ(res_bool, true);
	x *= 3;
	res_bool = mutex.Unlock();
	ASSERT_EQ(res_bool, true);
	EXPECT_TRUE(x == 18 || x == 16);
	x = 5;
}

TEST_F(Thread, amf_delete_mutex) {
	res_bool = amf_delete_mutex(mutex2);
	ASSERT_EQ(res_bool, true);
	res_bool = amf_wait_for_mutex(mutex2, 100);
	ASSERT_EQ(res_bool, true);
}

TEST_F(Thread, amf_release_mutex) {
	res_bool = amf_wait_for_mutex(mutex2, 100);
	ASSERT_EQ(res_bool, true);
	res_bool = amf_release_mutex(mutex2);
	ASSERT_EQ(res_bool, true);
}

TEST_F(Thread, amf_create_semaphore) {
}

TEST_F(Thread, amf_delete_semaphore) {
	res_bool = amf_delete_semaphore(semaphore);
	ASSERT_EQ(res_bool, true);
}

TEST_F(Thread, amf_wait_for_semaphore) {
	res_bool = amf_delete_semaphore(semaphore);
	ASSERT_EQ(res_bool, true);
}

TEST_F(Thread, amf_release_semaphore_NotTaking) {
	res_bool = amf_wait_for_semaphore(semaphore, 20);
	ASSERT_EQ(res_bool, true);
	res_bool = amf_release_semaphore(semaphore, 1, 0);
	ASSERT_EQ(res_bool, true);
}

TEST_F(Thread, amf_release_semaphore_AfterTaking) {
	res_bool = amf_release_semaphore(semaphore, 1, 0);
	ASSERT_EQ(res_bool, true);
}

TEST_F(Thread, amf_sleep_2seconds) {
	amf_sleep(2000);
}

TEST_F(Thread, amf_high_precision_clock) {
	amf_pts clock = amf_high_precision_clock();
	ASSERT_NE(clock, NULL);
}

TEST_F(Thread, amf_increase_timer_precision) {
	amf_pts clock = amf_high_precision_clock();
	ASSERT_NE(clock, NULL);
	amf_increase_timer_precision();
}

TEST_F(Thread, amf_restore_timer_precision) {
	amf_pts clock = amf_high_precision_clock();
	ASSERT_NE(clock, NULL);
	amf_restore_timer_precision();
	amf_load_library
}
/*
TEST_F(Thread, amf_load_library) {
	amf_load_library(AMF_DLL_NAME);
}

TEST_F(Thread, amf_get_proc_address) {
}

TEST_F(Thread, amf_free_library) {
}

TEST_F(Thread, get_current_thread_id) {
}

TEST_F(Thread, amf_virtual_alloc) {
}

TEST_F(Thread, amf_virtual_free) {
}
*/
