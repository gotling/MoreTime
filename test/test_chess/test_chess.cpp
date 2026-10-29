#include <unity.h>

#include <TestHelpers.h>
#include <MyTime.h>
#include <Chess.h>

void setUp() {
  resetTo(&chessMode);
}

void tearDown() {}

// Set both clocks to five minutes and start with white to move
static void startFiveMinutes() {
  chessMode.minus();
  chessMode.minus();
  TEST_ASSERT_EQUAL_STRING("5:00", displayText.c_str());
  chessMode.action();
}

void test_opens_at_fifteen_minutes() {
  TEST_ASSERT_EQUAL_STRING("15:00", displayText.c_str());
  TEST_ASSERT_EQUAL(FontThin, displayFont);
}

void test_time_steps_and_minimum() {
  chessMode.plus();
  TEST_ASSERT_EQUAL_STRING("20:00", displayText.c_str());
  for (int i = 0; i < 6; i++)
    chessMode.minus();
  TEST_ASSERT_EQUAL_STRING("5:00", displayText.c_str());
}

void test_white_moves_first() {
  chessMode.action();
  advance(1000);
  TEST_ASSERT_EQUAL_STRING("< 14:59", displayText.c_str());
}

void test_switching_turns_adds_increment() {
  chessMode.action();
  advance(1000);

  // White used 1 s and gets 10 s, black has not used any time yet
  chessMode.minus();
  TEST_ASSERT_EQUAL_STRING("15:00 >", displayText.c_str());

  advance(1000);
  TEST_ASSERT_EQUAL_STRING("14:59 >", displayText.c_str());

  chessMode.action();
  TEST_ASSERT_EQUAL_STRING("< 15:09", displayText.c_str());
}

void test_pressing_same_side_twice_changes_nothing() {
  chessMode.action();
  advance(1000);
  chessMode.minus();
  advance(1000);
  chessMode.minus();
  TEST_ASSERT_EQUAL_STRING("14:59 >", displayText.c_str());

  // White still only gets one increment
  chessMode.action();
  TEST_ASSERT_EQUAL_STRING("< 15:09", displayText.c_str());
}

void test_waiting_player_clock_is_stopped() {
  startFiveMinutes();

  // White thinks for 2 minutes, black for 4, then white for 2 more.
  // Nobody has used 5 minutes, so the game must still be running.
  advance(120000);
  chessMode.minus();
  advance(240000);
  chessMode.action();
  advance(120000);

  TEST_ASSERT_EQUAL(0, piezoPlays);
  // White: 300 - 120 + 10 - 120 = 70 s left
  TEST_ASSERT_EQUAL_STRING("< 1:10", displayText.c_str());
}

void test_game_over_when_time_runs_out() {
  startFiveMinutes();
  advance(300000);
  TEST_ASSERT_GREATER_THAN(0, piezoPlays);
  TEST_ASSERT_EQUAL_STRING("Black", displayText.c_str());
  TEST_ASSERT_EQUAL(FontDefault, displayFont);
}

void test_black_wins_when_white_runs_out_on_black_turn() {
  startFiveMinutes();
  chessMode.minus();
  advance(300000);
  TEST_ASSERT_EQUAL_STRING("White", displayText.c_str());
}

void test_minus_after_game_over_is_ignored() {
  startFiveMinutes();
  advance(300000);
  int plays = piezoPlays;

  chessMode.minus();
  TEST_ASSERT_EQUAL(plays, piezoPlays);
  TEST_ASSERT_EQUAL_STRING("Black", displayText.c_str());
}

void test_action_after_game_over_returns_to_setup() {
  startFiveMinutes();
  advance(300000);

  chessMode.action();
  TEST_ASSERT_EQUAL_STRING("5:00", displayText.c_str());
  advance(1000);
  TEST_ASSERT_EQUAL_STRING("5:00", displayText.c_str());

  // A new game starts with white to move, whoever moved last
  chessMode.action();
  advance(1000);
  TEST_ASSERT_EQUAL_STRING("< 4:59", displayText.c_str());
}

void test_new_game_starts_with_white() {
  startFiveMinutes();
  chessMode.minus();
  chessMode.hold();
  modeSwitch(&chessMode);

  chessMode.action();
  advance(1000);
  TEST_ASSERT_EQUAL_STRING("< 14:59", displayText.c_str());
}

void test_hold_resets_then_returns_to_clock() {
  chessMode.action();
  advance(1000);
  chessMode.hold();
  TEST_ASSERT_EQUAL_STRING("0:00", displayText.c_str());
  TEST_ASSERT_EQUAL_PTR(&chessMode, modeCurrent());

  chessMode.hold();
  TEST_ASSERT_EQUAL_PTR(&timeMode, modeCurrent());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_opens_at_fifteen_minutes);
  RUN_TEST(test_time_steps_and_minimum);
  RUN_TEST(test_white_moves_first);
  RUN_TEST(test_switching_turns_adds_increment);
  RUN_TEST(test_pressing_same_side_twice_changes_nothing);
  RUN_TEST(test_waiting_player_clock_is_stopped);
  RUN_TEST(test_game_over_when_time_runs_out);
  RUN_TEST(test_black_wins_when_white_runs_out_on_black_turn);
  RUN_TEST(test_minus_after_game_over_is_ignored);
  RUN_TEST(test_action_after_game_over_returns_to_setup);
  RUN_TEST(test_new_game_starts_with_white);
  RUN_TEST(test_hold_resets_then_returns_to_clock);
  return UNITY_END();
}
