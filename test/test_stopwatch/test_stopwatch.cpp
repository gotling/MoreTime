#include <unity.h>

#include <TestHelpers.h>
#include <MyTime.h>
#include <StopWatch.h>

void setUp() {
  resetTo(&stopWatchMode);
}

void tearDown() {}

void test_starts_running_on_open() {
  advance(1500);
  TEST_ASSERT_EQUAL_STRING("1.5", displayText.c_str());
  TEST_ASSERT_EQUAL(FontWide, displayFont);
}

void test_plus_pauses_and_resumes() {
  advance(1500);
  stopWatchMode.plus();
  advance(1000);
  TEST_ASSERT_EQUAL_STRING("1.5", displayText.c_str());

  stopWatchMode.plus();
  advance(500);
  TEST_ASSERT_EQUAL_STRING("2.0", displayText.c_str());
}

void test_shows_minutes() {
  advance(61000);
  TEST_ASSERT_EQUAL_STRING("1:01", displayText.c_str());
}

void test_shows_hours_in_thin_font() {
  advance(3600000);
  TEST_ASSERT_EQUAL_STRING("1:00:00", displayText.c_str());
  TEST_ASSERT_EQUAL(FontThin, displayFont);
}

void test_action_resets_then_returns_to_clock() {
  advance(1500);
  stopWatchMode.action();
  TEST_ASSERT_EQUAL_STRING("0.0", displayText.c_str());
  advance(1000);
  TEST_ASSERT_EQUAL_STRING("0.0", displayText.c_str());

  stopWatchMode.action();
  TEST_ASSERT_EQUAL_PTR(&timeMode, modeCurrent());
}

void test_remote_running_paused_and_reset() {
  advance(1500);
  RemoteState state = stopWatchMode.remoteState();
  TEST_ASSERT_EQUAL(RemoteState::StopWatch, state.kind);
  TEST_ASSERT_TRUE(state.running);
  TEST_ASSERT_EQUAL(1500, state.ms);

  stopWatchMode.plus();
  advance(1000);
  state = stopWatchMode.remoteState();
  TEST_ASSERT_FALSE(state.running);
  TEST_ASSERT_EQUAL(1500, state.ms);

  stopWatchMode.action();
  TEST_ASSERT_EQUAL(RemoteState::None, stopWatchMode.remoteState().kind);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_starts_running_on_open);
  RUN_TEST(test_plus_pauses_and_resumes);
  RUN_TEST(test_shows_minutes);
  RUN_TEST(test_shows_hours_in_thin_font);
  RUN_TEST(test_action_resets_then_returns_to_clock);
  RUN_TEST(test_remote_running_paused_and_reset);
  return UNITY_END();
}
