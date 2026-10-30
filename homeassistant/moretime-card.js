// Compact Home Assistant card for a MoreTime device. One row: the running
// timer or stopwatch ticking (tap to stop) and a field where typing minutes and
// pressing Enter starts a new timer. Needs the sensor and rest commands from
// moretime.yaml.
//
//   type: custom:moretime-card
//   entity: sensor.moretime
//   prefix: moretime          # optional, the rest_command name prefix

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

// "5" is five minutes, "1.5" and "1:30" are a minute and a half
function parseSeconds(text) {
  const value = text.trim().replace(",", ".");
  const parts = value.split(":");
  if (parts.length === 2 && parts[1].length) return Math.round(parseFloat(parts[0] || "0") * 60 + parseFloat(parts[1]));
  return Math.round(parseFloat(value) * 60);
}

// The sensor is polled, so a fresh card trusts last_updated for at most this long
const MAX_AGE = 10000;

class MoreTimeCard extends HTMLElement {
  setConfig(config) {
    if (!config.entity) throw new Error("Set entity, for example sensor.moretime");
    this.config = { prefix: "moretime", ...config };
  }

  set hass(hass) {
    this._hass = hass;
    const stateObj = hass.states[this.config.entity];
    if (stateObj && stateObj === this.stateObj) return;

    // Measure age with the browser's own clock once the card is running, the
    // server clock may differ from a wall tablet's
    const now = Date.now();
    this.receivedAt = this.stateObj
      ? now
      : now - Math.min(Math.max(now - Date.parse(stateObj?.last_updated), 0), MAX_AGE);
    this.stateObj = stateObj;
    this.build();
    this.render();
  }

  connectedCallback() {
    this.ticker = setInterval(() => this.render(), 250);
  }

  disconnectedCallback() {
    clearInterval(this.ticker);
  }

  getCardSize() {
    return 1;
  }

  getGridOptions() {
    return { rows: 1, columns: 6, min_columns: 4 };
  }

  build() {
    if (this.shadowRoot) return;
    this.attachShadow({ mode: "open" }).innerHTML = `
      <style>
        ha-card { display: flex; align-items: center; gap: 8px; height: 100%; box-sizing: border-box; padding: 0 8px 0 12px; min-height: 56px; }
        ha-icon { color: var(--state-icon-color); flex: none; }
        #running { display: flex; align-items: center; gap: 4px; flex: none; border: none; background: none; padding: 4px 8px; border-radius: 8px; cursor: pointer; color: var(--primary-text-color); font: inherit; }
        #running:hover { background: var(--secondary-background-color); }
        #running ha-icon { color: var(--error-color); --mdc-icon-size: 20px; }
        #time { font-size: 1.5em; font-weight: 500; font-variant-numeric: tabular-nums; }
        .paused #time { opacity: 0.5; }
        .over #time { color: var(--error-color); }
        form { flex: 1; min-width: 0; display: flex; }
        input { width: 100%; min-width: 0; box-sizing: border-box; height: 36px; padding: 0 10px; border-radius: 8px; border: 1px solid var(--divider-color); background: var(--card-background-color); color: var(--primary-text-color); font: inherit; font-size: 1.1em; }
        input:focus { outline: 2px solid var(--primary-color); outline-offset: -1px; }
        input:disabled { opacity: 0.5; }
        [hidden] { display: none !important; }
      </style>
      <ha-card>
        <ha-icon icon="mdi:timer-outline"></ha-icon>
        <button id="running" title="Stop" hidden>
          <span id="time"></span>
          <ha-icon icon="mdi:stop"></ha-icon>
        </button>
        <form>
          <input id="minutes" type="text" inputmode="decimal" enterkeyhint="go" autocomplete="off" placeholder="Minutes">
        </form>
      </ha-card>`;

    const root = this.shadowRoot;
    this.$ = (id) => root.getElementById(id);
    this.$("running").addEventListener("click", () => this.stop());
    root.querySelector("form").addEventListener("submit", (event) => {
      event.preventDefault();
      this.start();
    });
  }

  render() {
    if (!this.shadowRoot) return;
    const { state, attributes } = this.stateObj ?? { state: "unavailable", attributes: {} };
    const running = attributes.running;
    const elapsed = running ? Date.now() - this.receivedAt : 0;
    let time = null;
    let over = false;

    if (state === "timer") {
      const left = seconds(attributes.ms - elapsed);
      time = left >= 0 ? duration(left) : "+" + duration(-left);
      over = left <= 0;
    } else if (state === "stopwatch") {
      time = duration(Math.floor((attributes.ms + elapsed) / 1000));
    }

    const button = this.$("running");
    button.hidden = time === null;
    button.className = (running ? "" : "paused ") + (over ? "over" : "");
    this.$("time").textContent = time ?? "";

    const offline = state === "unavailable" || state === "unknown";
    const input = this.$("minutes");
    input.disabled = offline;
    input.placeholder = offline ? "Offline" : time === null ? "Minutes" : "New";
  }

  async call(command, data = {}) {
    try {
      await this._hass.callService("rest_command", `${this.config.prefix}_${command}`, data);
      await this._hass.callService("homeassistant", "update_entity", { entity_id: this.config.entity });
    } catch (error) {
      this.dispatchEvent(new CustomEvent("hass-notification", {
        detail: { message: `MoreTime: ${error.message || error}` },
        bubbles: true,
        composed: true,
      }));
    }
  }

  start() {
    const input = this.$("minutes");
    const secs = parseSeconds(input.value);
    if (!(secs > 0)) return;
    input.value = "";
    input.blur();
    this.call("timer_start", { seconds: secs });
  }

  stop() {
    this.call(`${this.stateObj.state}_stop`);
  }
}

customElements.define("moretime-card", MoreTimeCard);
window.customCards = window.customCards || [];
window.customCards.push({ type: "moretime-card", name: "MoreTime", description: "Compact timer for a MoreTime device" });
