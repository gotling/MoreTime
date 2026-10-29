#include <unity.h>

#include <string.h>

#include <Remote.h>

void setUp() {}
void tearDown() {}

static RemoteState roundTrip(const RemoteState &state, char *id) {
  char json[192];
  TEST_ASSERT_TRUE(remoteEncode(state, "a1b2c3", json, sizeof(json)));

  RemoteState decoded;
  TEST_ASSERT_TRUE(remoteDecode(json, decoded, id, 9));
  return decoded;
}

void test_timer_round_trip() {
  RemoteState state;
  state.kind = RemoteState::Timer;
  state.running = true;
  state.ms = -123456;

  char id[9];
  RemoteState decoded = roundTrip(state, id);
  TEST_ASSERT_EQUAL_STRING("a1b2c3", id);
  TEST_ASSERT_EQUAL(RemoteState::Timer, decoded.kind);
  TEST_ASSERT_TRUE(decoded.running);
  TEST_ASSERT_EQUAL(-123456, decoded.ms);
}

void test_chess_round_trip() {
  RemoteState state;
  state.kind = RemoteState::Chess;
  state.ms = 1000;
  state.blackMs = 250000;
  state.white = false;
  state.over = true;

  char id[9];
  RemoteState decoded = roundTrip(state, id);
  TEST_ASSERT_EQUAL(RemoteState::Chess, decoded.kind);
  TEST_ASSERT_FALSE(decoded.running);
  TEST_ASSERT_EQUAL(1000, decoded.ms);
  TEST_ASSERT_EQUAL(250000, decoded.blackMs);
  TEST_ASSERT_FALSE(decoded.white);
  TEST_ASSERT_TRUE(decoded.over);
}

void test_none_round_trip() {
  char id[9];
  RemoteState decoded = roundTrip(RemoteState(), id);
  TEST_ASSERT_EQUAL(RemoteState::None, decoded.kind);
}

void test_rejects_invalid_messages() {
  RemoteState state;
  char id[9];
  TEST_ASSERT_FALSE(remoteDecode("not json", state, id, sizeof(id)));
  TEST_ASSERT_FALSE(remoteDecode("{\"mode\":\"timer\"}", state, id, sizeof(id)));
  TEST_ASSERT_FALSE(remoteDecode("{\"id\":\"x\",\"mode\":\"rocket\"}", state, id, sizeof(id)));
}

void test_long_sender_id_is_cut() {
  RemoteState state;
  char id[9];
  TEST_ASSERT_TRUE(remoteDecode("{\"id\":\"0123456789abc\",\"mode\":\"none\"}", state, id, sizeof(id)));
  TEST_ASSERT_EQUAL_STRING("01234567", id);
}

void test_changed() {
  RemoteState a, b;
  a.kind = b.kind = RemoteState::Timer;
  a.running = b.running = true;
  a.ms = 5000;
  b.ms = 4000;
  // Running times differ all the time, the heartbeat covers it
  TEST_ASSERT_FALSE(remoteChanged(a, b));

  b.running = false;
  TEST_ASSERT_TRUE(remoteChanged(a, b));

  // Paused time changed with minus/plus
  a.running = false;
  TEST_ASSERT_TRUE(remoteChanged(a, b));

  a.ms = b.ms;
  TEST_ASSERT_FALSE(remoteChanged(a, b));

  b.kind = RemoteState::Chess;
  TEST_ASSERT_TRUE(remoteChanged(a, b));
}

void test_seconds_match_display_rounding() {
  TEST_ASSERT_EQUAL(60, remoteSeconds(59500));
  TEST_ASSERT_EQUAL(59, remoteSeconds(59000));
  TEST_ASSERT_EQUAL(1, remoteSeconds(1));
  TEST_ASSERT_EQUAL(0, remoteSeconds(0));
  TEST_ASSERT_EQUAL(0, remoteSeconds(-999));
  TEST_ASSERT_EQUAL(-1, remoteSeconds(-1000));
  TEST_ASSERT_EQUAL(-1, remoteSeconds(-1500));
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_timer_round_trip);
  RUN_TEST(test_chess_round_trip);
  RUN_TEST(test_none_round_trip);
  RUN_TEST(test_rejects_invalid_messages);
  RUN_TEST(test_long_sender_id_is_cut);
  RUN_TEST(test_changed);
  RUN_TEST(test_seconds_match_display_rounding);
  return UNITY_END();
}
