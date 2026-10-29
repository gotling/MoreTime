#include <unity.h>

#include <TestHelpers.h>
#include <MyTime.h>
#include <Timer.h>
#include <StopWatch.h>
#include <Control.h>

#define NO_SECONDS -1

void setUp() {
  resetTo(&timeMode);
}

void tearDown() {}

void test_timer_start_from_clock() {
  TEST_ASSERT_EQUAL(ControlOk, controlRun("timer/start", 300));
  TEST_ASSERT_EQUAL_PTR(&timerMode, modeCurrent());
  TEST_ASSERT_EQUAL_STRING("5:00", displayText.c_str());

  advance(1000);
  TEST_ASSERT_EQUAL_STRING("4:59", displayText.c_str());

  RemoteState state = controlState();
  TEST_ASSERT_EQUAL(RemoteState::Timer, state.kind);
  TEST_ASSERT_TRUE(state.running);
  TEST_ASSERT_EQUAL(299000, state.ms);
}

void test_timer_start_replaces_running_timer() {
  controlRun("timer/start", 300);
  advance(1000);
  controlRun("timer/start", 60);
  TEST_ASSERT_EQUAL_STRING("1:00", displayText.c_str());
  TEST_ASSERT_EQUAL(60000, controlState().ms);
}

void test_timer_start_needs_valid_seconds() {
  TEST_ASSERT_EQUAL(ControlInvalid, controlRun("timer/start", NO_SECONDS));
  TEST_ASSERT_EQUAL(ControlInvalid, controlRun("timer/start", 0));
  TEST_ASSERT_EQUAL(ControlInvalid, controlRun("timer/start", CONTROL_MAX_SECONDS + 1));
  TEST_ASSERT_EQUAL_PTR(&timeMode, modeCurrent());
}

void test_timer_pause_and_resume() {
  controlRun("timer/start", 60);
  advance(20000);
  TEST_ASSERT_EQUAL(ControlOk, controlRun("timer/pause", NO_SECONDS));
  TEST_ASSERT_FALSE(controlState().running);
  TEST_ASSERT_EQUAL_STRING("0:40", displayText.c_str());

  // Pausing twice changes nothing
  controlRun("timer/pause", NO_SECONDS);
  TEST_ASSERT_EQUAL(40000, controlState().ms);

  advance(5000);
  TEST_ASSERT_EQUAL(ControlOk, controlRun("timer/resume", NO_SECONDS));
  advance(1000);
  TEST_ASSERT_EQUAL_STRING("0:39", displayText.c_str());
}

void test_timer_stop_returns_to_clock() {
  controlRun("timer/start", 60);
  TEST_ASSERT_EQUAL(ControlOk, controlRun("timer/stop", NO_SECONDS));
  TEST_ASSERT_EQUAL_PTR(&timeMode, modeCurrent());
  TEST_ASSERT_EQUAL(RemoteState::None, controlState().kind);
}

void test_timer_commands_need_timer_on_screen() {
  TEST_ASSERT_EQUAL(ControlInactive, controlRun("timer/pause", NO_SECONDS));
  TEST_ASSERT_EQUAL(ControlInactive, controlRun("timer/resume", NO_SECONDS));
  TEST_ASSERT_EQUAL(ControlInactive, controlRun("timer/stop", NO_SECONDS));
}

void test_stopwatch_start_pause_resume_stop() {
  TEST_ASSERT_EQUAL(ControlOk, controlRun("stopwatch/start", NO_SECONDS));
  TEST_ASSERT_EQUAL_PTR(&stopWatchMode, modeCurrent());
  advance(1500);
  TEST_ASSERT_EQUAL_STRING("1.5", displayText.c_str());

  TEST_ASSERT_EQUAL(ControlOk, controlRun("stopwatch/pause", NO_SECONDS));
  advance(1000);
  TEST_ASSERT_EQUAL(1500, controlState().ms);

  TEST_ASSERT_EQUAL(ControlOk, controlRun("stopwatch/resume", NO_SECONDS));
  advance(500);
  TEST_ASSERT_EQUAL_STRING("2.0", displayText.c_str());

  TEST_ASSERT_EQUAL(ControlOk, controlRun("stopwatch/stop", NO_SECONDS));
  TEST_ASSERT_EQUAL_PTR(&timeMode, modeCurrent());
}

void test_stopwatch_start_restarts_from_zero() {
  controlRun("stopwatch/start", NO_SECONDS);
  advance(5000);
  controlRun("stopwatch/start", NO_SECONDS);
  advance(500);
  TEST_ASSERT_EQUAL_STRING("0.5", displayText.c_str());
}

void test_stopwatch_commands_need_stopwatch_on_screen() {
  TEST_ASSERT_EQUAL(ControlInactive, controlRun("stopwatch/pause", NO_SECONDS));
  TEST_ASSERT_EQUAL(ControlInactive, controlRun("stopwatch/stop", NO_SECONDS));
}

void test_unknown_commands() {
  TEST_ASSERT_EQUAL(ControlUnknown, controlRun("timer/explode", NO_SECONDS));
  TEST_ASSERT_EQUAL(ControlUnknown, controlRun("chess/start", NO_SECONDS));
  TEST_ASSERT_EQUAL(ControlUnknown, controlRun("", NO_SECONDS));
}

void test_state_when_idle() {
  TEST_ASSERT_EQUAL(RemoteState::None, controlState().kind);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_timer_start_from_clock);
  RUN_TEST(test_timer_start_replaces_running_timer);
  RUN_TEST(test_timer_start_needs_valid_seconds);
  RUN_TEST(test_timer_pause_and_resume);
  RUN_TEST(test_timer_stop_returns_to_clock);
  RUN_TEST(test_timer_commands_need_timer_on_screen);
  RUN_TEST(test_stopwatch_start_pause_resume_stop);
  RUN_TEST(test_stopwatch_start_restarts_from_zero);
  RUN_TEST(test_stopwatch_commands_need_stopwatch_on_screen);
  RUN_TEST(test_unknown_commands);
  RUN_TEST(test_state_when_idle);
  return UNITY_END();
}
