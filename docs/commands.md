# ROCBoard Command Reference

All commands are JSON objects sent over serial (Phase 4) or Wi-Fi/BLE (Phase 5+).

## Command Format

```json
{ "cmd": "COMMAND_NAME", ...parameters }
```

---

## Commands

### PING
Check connectivity. Returns PONG.
```json
{ "cmd": "PING" }
```
Response: `{"ok":true,"response":"PONG"}`

---

### SET_SCORE
Update score for one or both teams.
```json
{ "cmd": "SET_SCORE", "teamA": 8, "teamB": 6 }
```
- `teamA`: integer 0–99
- `teamB`: integer 0–99

---

### SET_NAME
Set a team's display name (up to 10 characters).
```json
{ "cmd": "SET_NAME", "team": "A", "name": "JOHN MIKE" }
{ "cmd": "SET_NAME", "team": "B", "name": "DAVIDPETER" }
```
- `team`: "A" or "B"
- `name`: string, max 10 characters (truncated if longer)

---

### CLEAR
Clear all display content.
```json
{ "cmd": "CLEAR" }
```

---

## Future Commands (Phase 5+)

```json
{ "cmd": "SET_SERVING", "team": "A" }
{ "cmd": "SET_GAME", "game": 2 }
{ "cmd": "SET_BRIGHTNESS", "value": 80 }
{ "cmd": "OTA_UPDATE", "url": "http://..." }
{ "cmd": "REBOOT" }
{ "cmd": "STATUS" }
```
