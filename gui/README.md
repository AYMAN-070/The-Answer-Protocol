# GUI Client — Technical Documentation

This document describes the internal architecture of the GUI client (`client_gui`),
built with [raylib](https://www.raylib.com/). It is written for whoever maintains
or extends this code — teammate, peer-reviewer, or future you.

The root [`README.md`](../README.md) covers the project as a whole (build
instructions, protocol deviations, group contributions). This file only covers
`gui/`.

## 1. Directory layout

```
gui/
├── include/          # one header per module, plus vendored raylib.h / cJSON.h
├── lib/              # vendored raylib + cJSON sources (see install-deps)
├── src/              # implementation, one .c per module (5-function norm cap)
└── README.md         # this file
```

Every `.c` file is capped at 5 functions by the 42 norm. Whenever a module
outgrows that cap, the overflow is split into a `<module>_utils.c` companion
file rather than crammed into one function — see `action_panel_utils.c`,
`chat_panel_utils.c`, `network_parser_utils.c`, `room_entities_utils.c`.

## 2. Runtime flow

```
main()
 ├─ init_client()          window, textures, world.json, TCP connect, CONNECT+WHO
 └─ run_client()            while (!WindowShouldClose())
     ├─ net_receive_state()      non-blocking recv, buffers partial lines
     └─ render_main_layout()     compute panel rects, draw everything
```

`main.c` owns the two long-lived objects: `t_game_assets` (textures) and
`t_ui_context` (everything else — connection, parsed world state, UI
selection state). Both are stack-allocated in `main()` and passed by pointer
everywhere; nothing is heap-allocated except the `cJSON` tree for the local
world file and the short-lived JSON payloads parsed per network message
(both freed where they are created).

`render_main_layout()` (`layout.c`) is the single place that lays out the
four HUD quadrants (room / inventory & status / chat / logs) and calls each
panel's `render_*` entry point once per frame. It also draws the red
"DISCONNECTED" banner when `ctx->connected` is false.

## 3. Core data structures (`layout.h`)

- **`t_ui_context`** — the god-object passed to almost every function.
  Holds the socket, the three chat buffers (`chat`/`global_chat`/`group_chat`),
  the log buffer, the text-input state, UI selection indices
  (`sel_item`/`sel_room_item`/`sel_npc`), the raw receive buffer (`rx_buf`,
  for reassembling TCP-fragmented lines), and the two "domain" structs below.
- **`t_room_state`** — the currently known room: name, description, exits,
  items/NPCs present, plus `key` (the room's canonical id, used to match
  `WHO` responses to "am I in this room") and the NPC-dialogue overlay
  fields (`current_npc`/`current_dialogue`).
- **`t_player_state`** — HP, inventory, and the `known_rooms` history used to
  fog-of-war unexplored areas on first `LOOK`.
- **`t_message_buffer`** (`chat_panel.h`) — a fixed ring of 15 lines, shared
  by chat and logs. `add_message()` shifts the oldest line out once full.
- **`t_game_assets`** / **`t_room_assets`** / **`t_item_assets`** /
  **`t_npc_assets`** (`graphics.h`) — preloaded `Texture2D` per room / item /
  NPC id, loaded once at startup and released at exit.

## 4. Network layer

`network.c` owns the raw socket. `net_receive_state()` does a single
non-blocking `recv()` per frame, appends the bytes to `ctx->rx_buf`
(dropping any control byte other than `\n` — a defensive filter against a
stray `\0` or `\r` from the server), and only hands **complete** lines to
`parse_server_text()`. Return value: `1` = data processed, `0` = nothing to
read, `-1` = connection lost (triggers the disconnect banner).

### 4.1 Dual protocol support — the most important design decision here

The server this client talks to is mid-migration from an ad-hoc line
protocol to the RFC 42TAP format. Rather than block on that migration, the
parser tries **both** formats on every line:

```
parse_server_text()                       (network_parser.c)
 └─ for each line:
     ├─ rfc_route_line(line, ctx)          try RFC format first
     │    └─ if handled, done
     └─ else: legacy fallback
          ├─ parse_player_state()          STATUS: / INVENTORY: / DEAD:
          ├─ fetch_room_data()             "Current Room : X"
          ├─ route_state_line()            WHO / DROPPED:
          └─ route_message()               NPC: / COMBAT: / EVENT: / *CHAT:
```

- **RFC path** (`rfc_parser.c`, `rfc_room.c`, `rfc_payload.c`): dispatches on
  the `OK` / `ERR <code>` / `EVT <type>` envelope from RFC 42TAP §4.1, then
  decodes the JSON payloads (`rfc_apply_room`, `rfc_apply_status`,
  `rfc_apply_combat`, `rfc_apply_quest`, `rfc_apply_list`).
- **Legacy path** (`network_parser.c`, `network_parser_utils.c`): the
  original ad-hoc format (`Current Room : X`, `INVENTORY:a,b`,
  `STATUS:HP:80/100 ROOM:x`, `NPC:`, `COMBAT:`, …). It also re-reads
  `data/world.json` locally for room contents, since the legacy `LOOK`
  response never carried that data.

**Both paths update the exact same `t_ui_context` fields**, so every panel
renders identically regardless of which format the server actually speaks.
This means the server team can migrate one command at a time — nothing on
the client breaks mid-migration, and nothing needs to change here again
until the legacy path is deleted outright.

Outgoing commands are always sent in the RFC form (`CHAT ROOM …`,
`GROUP CREATE`, …) from `chat_panel_utils.c` / `action_panel.c` — the
legacy/RFC duality only applies to *parsing* the server's replies.

### 4.2 "Confirm" helpers (`player_confirm.c`)

Several commands (`STATUS`, `INVENTORY`, `WHO`) update state silently by
design in both protocol paths. `confirm_status()` / `confirm_inventory()` /
`confirm_who()` centralize the one-line log message that proves the
round-trip happened (e.g. `Status: HP 80/100`), called from both the legacy
parser and the RFC payload decoder so the message text stays consistent.

## 5. Module reference

| File | Responsibility |
|---|---|
| `main.c` | Entry point, init/loop/teardown, local `world.json` loading |
| `network.c` | Socket connect/send/receive, line-buffering, disconnect detection |
| `network_parser.c` / `network_parser_utils.c` | Legacy protocol decoding |
| `rfc_parser.c` / `rfc_room.c` / `rfc_payload.c` | RFC 42TAP protocol decoding |
| `player_confirm.c` | Shared "your command worked" log messages |
| `layout.c` | HUD quadrant layout, top-level per-frame render call |
| `room_view.c` | Room background, NPC dialogue overlay, unexplored-room dimming, player counters |
| `room_entities.c` / `room_entities_utils.c` | NPC & room-item sprites, click-to-select, missing-texture fallback |
| `map.c` | Fixed-layout minimap of the 8 rooms |
| `status_panel.c` | HP bar, clickable inventory list |
| `action_panel.c` / `action_panel_utils.c` | The 17 command buttons, button enablement rules |
| `chat_panel.c` / `chat_panel_utils.c` | Room/Global/Group tabs, text input, `/group` sub-commands |
| `graphics.c` / `graphics_items.c` / `graphics_npcs.c` | Texture preloading per room/item/NPC id |

## 6. Extending the client

**Adding a new RFC command's response format:** add a branch in
`rfc_apply_json()` (`rfc_room.c`) if it's a new payload shape, or a new
`else if` in `route_ok()` / `route_evt()` (`rfc_parser.c`) if it's a
`key=value` reply or event. Keep the 5-function/25-line caps in mind — split
into a new `rfc_*.c` file (and add it to the root `Makefile`'s `GUI_SRC`) if
a file is already full.

**Migrating a command off the legacy format:** once the server sends the RFC
form for a given command, the corresponding legacy branch
(`network_parser.c` / `network_parser_utils.c`) becomes dead code and can be
deleted — check first that no other legacy branch still depends on it.

**Adding a UI element:** panels are self-contained `render_*(bounds, ctx, …)`
functions called from `render_main_layout()`; they read `ctx` but never talk
to the network directly — outgoing commands go through `net_send_command()`
in `action_panel.c` / `chat_panel_utils.c` only.

## 7. Build & run

See the root `Makefile` (`make install-deps && make client_gui`,
`make run-client-gui`). No build step is specific to this directory beyond
what's already wired into `GUI_SRC`.

## 8. Known limitations (as of this writing)

- The room-player counter only updates on `WHO` (sent once at connect) or an
  `EVT ROOM PRESENCE` / RFC `LOOK` — it does not poll, by design (see git
  history for why periodic polling was removed).
- Chat is inert against the legacy server (it still expects `RCHAT`/`GCHAT`,
  the client sends RFC `CHAT ROOM/GLOBAL/GROUP`) until the server migrates
  `CHAT`.
- Item/NPC display names are resolved from the locally-loaded
  `data/world.json`, not from the server — a server-unknown id falls back to
  displaying the raw id.
