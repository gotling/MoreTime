#include <unity.h>

#include <TestHelpers.h>
#include <MyTime.h>
#include <Timer.h>

void setUp() {
  resetTo(&timerMode);
}

void tearDown() {}

// Set the timer to one minute and start it
static void startOneMinute() {
  timerMode.plus();
  timerMode.plus();
  timerMode.action();
}

void test_opens_at_zero() {
  TEST_ASSERT_EQUAL_STRING("0:00", displayText.c_str());
  TEST_ASSERT_EQUAL(FontWide, displayFont);
}

void test_plus_minus_step_30_seconds() {
  timerMode.plus();
  timerMode.plus();
  TEST_ASSERT_EQUAL_STRING("1:00", displayText.c_str());
  timerMode.minus();
  TEST_ASSERT_EQUAL_STRING("0:30", displayText.c_str());
}

void test_minus_stops_at_zero() {
  timerMode.plus();
  timerMode.minus();
  timerMode.minus();
  TEST_ASSERT_EQUAL_STRING("0:00", displayText.c_str());
}

void test_action_at_zero_returns_to_clock() {
  timerMode.action();
  TEST_ASSERT_EQUAL_PTR(&timeMode, modeCurrent());
}

void test_counts_down() {
  startOneMinute();
  advance(1000);
  TEST_ASSERT_EQUAL_STRING("0:59", displayText.c_str());
}

void test_alarm_at_zero() {
  startOneMinute();
  advance(59000);
  TEST_ASSERT_EQUAL(0, piezoPlays);
  advance(1000);
  TEST_ASSERT_GREATER_THAN(0, piezoPlays);
  TEST_ASSERT_EQUAL_STRING("0:00", displayText.c_str());
}

void test_counts_overtime_after_alarm() {
  startOneMinute();
  advance(61000);
  TEST_ASSERT_EQUAL_STRING("+0:01", displayText.c_str());
  TEST_ASSERT_EQUAL(FontWide, displayFont);
}

void test_long_overtime_uses_thin_font() {
  startOneMinute();
  advance(660000);
  TEST_ASSERT_EQUAL_STRING("+10:00", displayText.c_str());
  TEST_ASSERT_EQUAL(FontThin, displayFont);
}

void test_pause_and_resume() {
  startOneMinute();
  advance(20000);
  timerMode.action();
  TEST_ASSERT_EQUAL_STRING("0:40", displayText.c_str());

  advance(5000);
  TEST_ASSERT_EQUAL_STRING("0:40", displayText.c_str());

  timerMode.action();
  advance(1000);
  TEST_ASSERT_EQUAL_STRING("0:39", displayText.c_str());
}

void test_pause_after_alarm_stops_at_zero() {
  startOneMinute();
  advance(65000);
  timerMode.action();
  TEST_ASSERT_EQUAL_STRING("0:00", displayText.c_str());

  timerMode.action();
  TEST_ASSERT_EQUAL_PTR(&timeMode, modeCurrent());
}

void test_hold_resets_then_returns_to_clock() {
  startOneMinute();
  advance(10000);
  timerMode.hold();
  TEST_ASSERT_EQUAL_STRING("0:00", displayText.c_str());
  TEST_ASSERT_EQUAL_PTR(&timerMode, modeCurrent());

  timerMode.hold();
  TEST_ASSERT_EQUAL_PTR(&timeMode, modeCurrent());
}

void test_an_hour_or_more_uses_thin_font() {
  for (int i = 0; i < 120; i++)
    timerMode.plus();
  TEST_ASSERT_EQUAL_STRING("1:00:00", displayText.c_str());
  TEST_ASSERT_EQUAL(FontThin, displayFont);
}

void test_reopening_starts_from_zero() {
  timerMode.plus();
  modeSwitch(&timeMode);
  modeSwitch(&timerMode);
  TEST_ASSERT_EQUAL_STRING("0:00", displayText.c_str());
}

void test_remote_none_until_started() {
  timerMode.plus();
  TEST_ASSERT_EQUAL(RemoteState::None, timerMode.remoteState().kind);
}

void test_remote_running() {
  startOneMinute();
  advance(1500);
  RemoteState state = timerMode.remoteState();
  TEST_ASSERT_EQUAL(RemoteState::Timer, state.kind);
  TEST_ASSERT_TRUE(state.running);
  TEST_ASSERT_EQUAL(58500, state.ms);
}

void test_remote_paused() {
  startOneMinute();
  advance(20000);
  timerMode.action();
  RemoteState state = timerMode.remoteState();
  TEST_ASSERT_FALSE(state.running);
  TEST_ASSERT_EQUAL(40000, state.ms);
}

void test_remote_none_after_reset() {
  startOneMinute();
  timerMode.hold();
  TEST_ASSERT_EQUAL(RemoteState::None, timerMode.remoteState().kind);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_opens_at_zero);
  RUN_TEST(test_plus_minus_step_30_seconds);
  RUN_TEST(test_minus_stops_at_zero);
  RUN_TEST(test_action_at_zero_returns_to_clock);
  RUN_TEST(test_counts_down);
  RUN_TEST(test_alarm_at_zero);
  RUN_TEST(test_counts_overtime_after_alarm);
  RUN_TEST(test_long_overtime_uses_thin_font);
  RUN_TEST(test_pause_and_resume);
  RUN_TEST(test_pause_after_alarm_stops_at_zero);
  RUN_TEST(test_hold_resets_then_returns_to_clock);
  RUN_TEST(test_an_hour_or_more_uses_thin_font);
  RUN_TEST(test_reopening_starts_from_zero);
  RUN_TEST(test_remote_none_until_started);
  RUN_TEST(test_remote_running);
  RUN_TEST(test_remote_paused);
  RUN_TEST(test_remote_none_after_reset);
  return UNITY_END();
}
