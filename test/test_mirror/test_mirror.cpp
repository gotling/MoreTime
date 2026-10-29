#include <unity.h>

#include <TestHelpers.h>
#include <MyTime.h>
#include <Counter.h>
#include <Mirror.h>

void setUp() {
  // Clear what the mirror remembers from the previous test
  mirrorMode.receive(RemoteState());
  resetTo(&timeMode);
}

void tearDown() {}

static RemoteState timer(long ms, bool running = true) {
  RemoteState state;
  state.kind = RemoteState::Timer;
  state.running = running;
  state.ms = ms;
  return state;
}

static RemoteState stopwatch(long ms) {
  RemoteState state;
  state.kind = RemoteState::StopWatch;
  state.running = true;
  state.ms = ms;
  return state;
}

static RemoteState chess(long whiteMs, long blackMs, bool white, bool over = false) {
  RemoteState state;
  state.kind = RemoteState::Chess;
  state.running = !over;
  state.ms = whiteMs;
  state.blackMs = blackMs;
  state.white = white;
  state.over = over;
  return state;
}

void test_takes_over_clock_when_timer_starts() {
  mirrorMode.receive(timer(60000));
  TEST_ASSERT_EQUAL_PTR(&mirrorMode, modeCurrent());
  TEST_ASSERT_EQUAL_STRING("1:00", displayText.c_str());
}

void test_counts_down_between_messages() {
  mirrorMode.receive(timer(60000));
  advance(1000);
  TEST_ASSERT_EQUAL_STRING("0:59", displayText.c_str());
}

void test_follows_new_messages() {
  mirrorMode.receive(timer(60000));
  advance(1000);
  mirrorMode.receive(timer(50000));
  TEST_ASSERT_EQUAL_STRING("0:50", displayText.c_str());
}

void test_paused_timer_stays() {
  mirrorMode.receive(timer(40000, false));
  advance(3000);
  TEST_ASSERT_EQUAL_STRING("0:40", displayText.c_str());
}

void test_returns_to_clock_when_stopped() {
  mirrorMode.receive(timer(60000));
  mirrorMode.receive(RemoteState());
  TEST_ASSERT_EQUAL_PTR(&timeMode, modeCurrent());
}

void test_returns_to_clock_when_messages_stop() {
  mirrorMode.receive(timer(60000));
  advance(4900);
  TEST_ASSERT_EQUAL_PTR(&mirrorMode, modeCurrent());
  advance(200);
  TEST_ASSERT_EQUAL_PTR(&timeMode, modeCurrent());
}

void test_alarm_once_when_timer_reaches_zero() {
  mirrorMode.receive(timer(2000));
  advance(1000);
  TEST_ASSERT_EQUAL(0, piezoPlays);

  advance(1000);
  TEST_ASSERT_EQUAL(1, piezoPlays);
  TEST_ASSERT_EQUAL_STRING("0:00", displayText.c_str());

  advance(1500);
  TEST_ASSERT_EQUAL(1, piezoPlays);
  TEST_ASSERT_EQUAL_STRING("+0:01", displayText.c_str());
}

void test_no_alarm_when_joining_after_zero() {
  mirrorMode.receive(timer(-5000));
  advance(1000);
  TEST_ASSERT_EQUAL(0, piezoPlays);
  TEST_ASSERT_EQUAL_STRING("+0:06", displayText.c_str());
}

void test_button_dismisses_until_next_session() {
  mirrorMode.receive(timer(60000));
  mirrorMode.action();
  TEST_ASSERT_EQUAL_PTR(&timeMode, modeCurrent());

  mirrorMode.receive(timer(59000));
  TEST_ASSERT_EQUAL_PTR(&timeMode, modeCurrent());

  mirrorMode.receive(RemoteState());
  mirrorMode.receive(timer(30000));
  TEST_ASSERT_EQUAL_PTR(&mirrorMode, modeCurrent());
}

void test_does_not_interrupt_other_modes() {
  modeSwitch(&counterMode);
  mirrorMode.receive(timer(60000));
  TEST_ASSERT_EQUAL_PTR(&counterMode, modeCurrent());
}

void test_stopwatch_counts_up() {
  mirrorMode.receive(stopwatch(1500));
  TEST_ASSERT_EQUAL_STRING("1.5", displayText.c_str());
  advance(1000);
  TEST_ASSERT_EQUAL_STRING("2.5", displayText.c_str());
}

void test_chess_shows_player_to_move() {
  mirrorMode.receive(chess(300000, 300000, true));
  TEST_ASSERT_EQUAL_STRING("< 5:00", displayText.c_str());
  advance(1000);
  TEST_ASSERT_EQUAL_STRING("< 4:59", displayText.c_str());

  mirrorMode.receive(chess(309000, 300000, false));
  TEST_ASSERT_EQUAL_STRING("5:00 >", displayText.c_str());
}

void test_chess_stops_at_zero_until_told_game_over() {
  mirrorMode.receive(chess(500, 300000, true));
  advance(1000);
  TEST_ASSERT_EQUAL_STRING("< 0:00", displayText.c_str());
}

void test_chess_game_over_shows_winner_and_beeps_once() {
  mirrorMode.receive(chess(1000, 250000, true));
  mirrorMode.receive(chess(0, 250000, true, true));
  TEST_ASSERT_EQUAL_STRING("Black", displayText.c_str());
  TEST_ASSERT_EQUAL(1, piezoPlays);

  mirrorMode.receive(chess(0, 250000, true, true));
  TEST_ASSERT_EQUAL(1, piezoPlays);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_takes_over_clock_when_timer_starts);
  RUN_TEST(test_counts_down_between_messages);
  RUN_TEST(test_follows_new_messages);
  RUN_TEST(test_paused_timer_stays);
  RUN_TEST(test_returns_to_clock_when_stopped);
  RUN_TEST(test_returns_to_clock_when_messages_stop);
  RUN_TEST(test_alarm_once_when_timer_reaches_zero);
  RUN_TEST(test_no_alarm_when_joining_after_zero);
  RUN_TEST(test_button_dismisses_until_next_session);
  RUN_TEST(test_does_not_interrupt_other_modes);
  RUN_TEST(test_stopwatch_counts_up);
  RUN_TEST(test_chess_shows_player_to_move);
  RUN_TEST(test_chess_stops_at_zero_until_told_game_over);
  RUN_TEST(test_chess_game_over_shows_winner_and_beeps_once);
  return UNITY_END();
}
