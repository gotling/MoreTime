#include <unity.h>

#include <TestHelpers.h>
#include <MyTime.h>
#include <Counter.h>

void setUp() {
  resetTo(&counterMode);
}

void tearDown() {}

void test_opens_at_zero() {
  TEST_ASSERT_EQUAL_STRING("0", displayText.c_str());
  TEST_ASSERT_EQUAL(FontWide, displayFont);
}

void test_counts_up_and_down() {
  counterMode.plus();
  counterMode.plus();
  counterMode.plus();
  counterMode.minus();
  TEST_ASSERT_EQUAL_STRING("2", displayText.c_str());
}

void test_counts_below_zero() {
  counterMode.minus();
  TEST_ASSERT_EQUAL_STRING("-1", displayText.c_str());
}

void test_action_resets_then_returns_to_clock() {
  counterMode.plus();
  counterMode.action();
  TEST_ASSERT_EQUAL_STRING("0", displayText.c_str());
  TEST_ASSERT_EQUAL_PTR(&counterMode, modeCurrent());

  counterMode.action();
  TEST_ASSERT_EQUAL_PTR(&timeMode, modeCurrent());
}

void test_hold_returns_to_clock() {
  counterMode.plus();
  counterMode.hold();
  TEST_ASSERT_EQUAL_PTR(&timeMode, modeCurrent());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_opens_at_zero);
  RUN_TEST(test_counts_up_and_down);
  RUN_TEST(test_counts_below_zero);
  RUN_TEST(test_action_resets_then_returns_to_clock);
  RUN_TEST(test_hold_returns_to_clock);
  return UNITY_END();
}
