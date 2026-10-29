#include <unity.h>

#include <Format.h>

void setUp() {}
void tearDown() {}

void test_minutes() {
  TEST_ASSERT_EQUAL_STRING("0:00", formatMinutes(0));
  TEST_ASSERT_EQUAL_STRING("0:59", formatMinutes(59));
  TEST_ASSERT_EQUAL_STRING("1:00", formatMinutes(60));
  TEST_ASSERT_EQUAL_STRING("60:00", formatMinutes(3600));
}

void test_duration_below_an_hour() {
  TEST_ASSERT_EQUAL_STRING("0:05", formatDuration(5));
  TEST_ASSERT_EQUAL_STRING("59:59", formatDuration(3599));
}

void test_duration_from_an_hour() {
  TEST_ASSERT_EQUAL_STRING("1:00:00", formatDuration(3600));
  TEST_ASSERT_EQUAL_STRING("1:01:01", formatDuration(3661));
  TEST_ASSERT_EQUAL_STRING("10:00:00", formatDuration(36000));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_minutes);
  RUN_TEST(test_duration_below_an_hour);
  RUN_TEST(test_duration_from_an_hour);
  return UNITY_END();
}
