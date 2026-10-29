#ifndef _PAGE_H_
#define _PAGE_H_

#include <pgmspace.h>

// Served at /. Self-contained so it works without internet access.
// Polls /api/state every second and counts locally in between, like the mirror.
static const char PAGE[] PROGMEM = R"PAGE(<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta name="theme-color" content="#16181d">
<title>MoreTime</title>
<style>
:root {
  --bg: #f4f4f1; --card: #ffffff; --text: #1d1f24; --muted: #6b6f78;
  --accent: #d9480f; --accent-text: #ffffff; --line: #e2e2dc; --button: #ecece6;
  color-scheme: light;
}
@media (prefers-color-scheme: dark) {
  :root {
    --bg: #16181d; --card: #20232a; --text: #f1f1ee; --muted: #9a9ea8;
    --accent: #ff7a3d; --accent-text: #16181d; --line: #2e323b; --button: #2b2f37;
    color-scheme: dark;
  }
}
* { box-sizing: border-box; }
body {
  margin: 0; padding: 20px 16px 40px; background: var(--bg); color: var(--text);
  font: 16px/1.4 system-ui, -apple-system, "Segoe UI", Roboto, sans-serif;
}
main { max-width: 440px; margin: 0 auto; display: grid; gap: 16px; }
.status {
  background: #16181d; color: #ff7a3d; border-radius: 16px; padding: 24px 16px 18px;
  text-align: center;
}
.status .time {
  font: 600 64px/1 ui-monospace, "SF Mono", Menlo, Consolas, monospace;
  font-variant-numeric: tabular-nums; letter-spacing: -1px;
}
.status .label { margin-top: 10px; color: #9a9ea8; font-size: 14px; }
.card { background: var(--card); border: 1px solid var(--line); border-radius: 16px; padding: 16px; }
h2 { margin: 0 0 12px; font-size: 15px; font-weight: 600; color: var(--muted); }
.presets { display: grid; grid-template-columns: repeat(5, 1fr); gap: 8px; }
.row { display: flex; gap: 8px; margin-top: 10px; }
.row > * { flex: 1; }
button, input {
  font: inherit; min-height: 48px; border-radius: 12px; border: 0;
  background: var(--button); color: var(--text);
}
button { font-weight: 600; cursor: pointer; }
button:active { transform: scale(0.97); }
button.primary { background: var(--accent); color: var(--accent-text); }
button[hidden] { display: none; }
input { padding: 0 12px; border: 1px solid var(--line); background: var(--card); min-width: 0; }
.error { color: var(--accent); text-align: center; min-height: 1.4em; font-size: 14px; }
footer { text-align: center; color: var(--muted); font-size: 13px; }
</style>
</head>
<body>
<main>
  <div class="status">
    <div class="time" id="time">--:--</div>
    <div class="label" id="label">Connecting</div>
  </div>

  <section class="card">
    <h2>Timer</h2>
    <div class="presets">
      <button data-minutes="1">1</button>
      <button data-minutes="3">3</button>
      <button data-minutes="5">5</button>
      <button data-minutes="10">10</button>
      <button data-minutes="15">15</button>
    </div>
    <form class="row" id="custom">
      <input id="minutes" type="number" inputmode="decimal" min="0.5" max="1440" step="0.5" placeholder="Minutes" aria-label="Minutes">
      <button class="primary" type="submit">Start</button>
    </form>
    <div class="row">
      <button id="timer-pause" hidden>Pause</button>
      <button id="timer-resume" class="primary" hidden>Resume</button>
      <button id="timer-stop" hidden>Stop</button>
    </div>
  </section>

  <section class="card">
    <h2>Stopwatch</h2>
    <div class="row">
      <button id="stopwatch-start" class="primary">Start</button>
      <button id="stopwatch-pause" hidden>Pause</button>
      <button id="stopwatch-resume" class="primary" hidden>Resume</button>
      <button id="stopwatch-stop" hidden>Stop</button>
    </div>
  </section>

  <div class="error" id="error"></div>
  <footer id="device"></footer>
</main>
<script>
let state = { mode: "none" };
let receivedAt = Date.now();
let errorTimer;

const $ = (id) => document.getElementById(id);

// Same rounding and formats as the device display
function seconds(ms) {
  return ms > 0 ? Math.ceil(ms / 1000) : -Math.floor(-ms / 1000);
}
function pad(n) {
  return String(n).padStart(2, "0");
}
function duration(s) {
  if (s >= 3600) return Math.floor(s / 3600) + ":" + pad(Math.floor(s % 3600 / 60)) + ":" + pad(s % 60);
  return Math.floor(s / 60) + ":" + pad(s % 60);
}

function elapsed() {
  return state.running ? Date.now() - receivedAt : 0;
}

function render() {
  const mode = state.mode;
  let time = "--:--";
  let label = "Showing the clock";

  if (mode === "timer") {
    const left = seconds(state.ms - elapsed());
    time = left >= 0 ? duration(left) : "+" + duration(-left);
    label = !state.running ? "Timer paused" : left > 0 ? "Timer running" : "Time is up";
  } else if (mode === "stopwatch") {
    const ms = state.ms + elapsed();
    time = ms < 60000 ? (ms / 1000).toFixed(1) : duration(Math.floor(ms / 1000));
    label = state.running ? "Stopwatch running" : "Stopwatch paused";
  } else if (mode === "chess") {
    time = "Chess";
    label = "Chess clock on the device";
  }

  $("time").textContent = time;
  $("label").textContent = label;

  $("timer-pause").hidden = !(mode === "timer" && state.running);
  $("timer-resume").hidden = !(mode === "timer" && !state.running);
  $("timer-stop").hidden = mode !== "timer";
  $("stopwatch-pause").hidden = !(mode === "stopwatch" && state.running);
  $("stopwatch-resume").hidden = !(mode === "stopwatch" && !state.running);
  $("stopwatch-stop").hidden = mode !== "stopwatch";
  $("stopwatch-start").textContent = mode === "stopwatch" ? "Restart" : "Start";
}

function showError(message) {
  $("error").textContent = message;
  clearTimeout(errorTimer);
  errorTimer = setTimeout(() => ($("error").textContent = ""), 4000);
}

function update(json) {
  state = json;
  receivedAt = Date.now();
  if (json.id) $("device").textContent = json.id;
  render();
}

async function refresh() {
  try {
    const response = await fetch("/api/state", { cache: "no-store" });
    update(await response.json());
  } catch {
    $("label").textContent = "Cannot reach the device";
  }
}

async function command(path, secs) {
  const url = "/api/" + path + (secs ? "?seconds=" + secs : "");
  try {
    const response = await fetch(url, { method: "POST" });
    const json = await response.json();
    if (response.ok) update(json);
    else showError(json.error || "Failed");
  } catch {
    showError("Cannot reach the device");
  }
}

document.querySelectorAll("[data-minutes]").forEach((button) =>
  button.addEventListener("click", () => command("timer/start", button.dataset.minutes * 60))
);
$("custom").addEventListener("submit", (event) => {
  event.preventDefault();
  const secs = Math.round(parseFloat($("minutes").value) * 60);
  if (secs > 0) command("timer/start", secs);
  else showError("Enter minutes");
});
for (const id of ["timer-pause", "timer-resume", "timer-stop",
                  "stopwatch-start", "stopwatch-pause", "stopwatch-resume", "stopwatch-stop"]) {
  $(id).addEventListener("click", () => command(id.replace("-", "/")));
}

refresh();
setInterval(refresh, 1000);
setInterval(render, 100);
</script>
</body>
</html>
)PAGE";

#endif
