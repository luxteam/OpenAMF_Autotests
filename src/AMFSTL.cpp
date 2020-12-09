#include "autotests.h"
#include "../../../common/AMFSTL.h"

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