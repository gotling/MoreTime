#include <unity.h>

#include <TestHelpers.h>
#include <MyTime.h>
#include <Rice.h>

#define INTERVAL 1500

void setUp() {
  resetTo(&riceMode);
}

void tearDown() {}

void test_opens_asking_for_units() {
  TEST_ASSERT_EQUAL_STRING("Units", displayText.c_str());
}

void test_minus_at_zero_is_ignored() {
  riceMode.minus();
  TEST_ASSERT_EQUAL_STRING("Units", displayText.c_str());
  riceMode.plus();
  TEST_ASSERT_EQUAL_STRING("0.5", displayText.c_str());
}

void test_minus_stops_at_half_unit() {
  riceMode.plus();
  riceMode.minus();
  TEST_ASSERT_EQUAL_STRING("0.5", displayText.c_str());

  riceMode.plus();
  riceMode.minus();
  TEST_ASSERT_EQUAL_STRING("0.5", displayText.c_str());
}

void test_cycles_units_rice_water() {
  riceMode.plus();
  riceMode.plus();
  TEST_ASSERT_EQUAL_STRING("1.0", displayText.c_str());
  TEST_ASSERT_EQUAL(FontWide, displayFont);

  const char *expected[] = {"Rice", "100", "Water", "180", "Units", "1.0"};
  for (const char *text : expected) {
    advance(INTERVAL);
    TEST_ASSERT_EQUAL_STRING(text, displayText.c_str());
  }
}

void test_change_restarts_cycle() {
  riceMode.plus();
  advance(INTERVAL * 2);
  TEST_ASSERT_EQUAL_STRING("50", displayText.c_str());

  riceMode.plus();
  TEST_ASSERT_EQUAL_STRING("1.0", displayText.c_str());
  // The new value stays for a full interval
  advance(INTERVAL - 10);
  TEST_ASSERT_EQUAL_STRING("1.0", displayText.c_str());
}

void test_half_units() {
  for (int i = 0; i < 3; i++)
    riceMode.plus();
  advance(INTERVAL * 2);
  TEST_ASSERT_EQUAL_STRING("150", displayText.c_str());
  advance(INTERVAL * 2);
  TEST_ASSERT_EQUAL_STRING("270", displayText.c_str());
}

void test_action_returns_to_clock_only_at_zero() {
  riceMode.plus();
  riceMode.action();
  TEST_ASSERT_EQUAL_PTR(&riceMode, modeCurrent());

  resetTo(&riceMode);
  riceMode.action();
  TEST_ASSERT_EQUAL_PTR(&timeMode, modeCurrent());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_opens_asking_for_units);
  RUN_TEST(test_minus_at_zero_is_ignored);
  RUN_TEST(test_minus_stops_at_half_unit);
  RUN_TEST(test_cycles_units_rice_water);
  RUN_TEST(test_change_restarts_cycle);
  RUN_TEST(test_half_units);
  RUN_TEST(test_action_returns_to_clock_only_at_zero);
  return UNITY_END();
}
