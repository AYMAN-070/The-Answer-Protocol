# GUI Client — Technical Documentation

This document describes the internal architecture of the GUI client (`client_gui`),
built with [raylib](https://www.raylib.com/). It is written for whoever maintains
or extends this code — teammate, peer-reviewer, or future you.

The root [`README.md`](../README.md) covers the project as a whole (build
instructions, protocol deviations, group contributions). [`CORRECTIONS.md`](CORRECTIONS.md)
records what was changed in this directory and why. [`../GAMEPLAY.md`](../GAMEPLAY.md)
is the player-facing guide. This file only covers how `gui/` is built.

## 1. Directory layout

```
gui/
├── include/          # one header per module, plus vendored raylib.h / cJSON.h
├── lib/              # vendored raylib + cJSON sources (see install-deps)
├── src/              # implementation, one .c per module (5-function norm cap)
├── CORRECTIONS.md    # change log for this directory
└── README.md         # this file
```

Every `.c` file is capped at 5 functions by the 42 norm. Whenever a module
outgrows that cap, the overflow is split into a `<module>_utils.c` companion
file rather than crammed into one function — see `action_panel_utils.c`,
`chat_panel_utils.c`, `room_entities_utils.c`.

## 2. Runtime flow

```
main(argc, argv)
 ├─ init_client()           window, textures, local world.json
 │   └─ connect_to_server()  TCP connect, then CONNECT + WHO + LOOK
 └─ run_client()             while (!WindowShouldClose())
     ├─ net_receive_state()      non-blocking recv, buffers partial lines
     └─ render_main_layout()     compute panel rects, draw everything
```

The name, IP and port are read from `argv` (`./client_gui [name] [ip] [port]`,
defaults `gui_user` / `127.0.0.1` / `4242`). Running two clients without
distinct names makes the server reject the second `CONNECT` with
`ERR 201 NAME_IN_USE`.

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
  for reassembling TCP-fragmented lines), the last command sent (`last_cmd`,
  used to disambiguate otherwise identical JSON replies), the de-duplication
  signatures (`last_inv_sig`/`last_quest_sig`, see §4.2), and the two "domain"
  structs below.
- **`t_room_state`** — the currently known room: name, description, exits,
  items/NPCs present, and the NPC-dialogue overlay fields
  (`current_npc`/`current_dialogue`).
- **`t_player_state`** — HP, inventory, and the `known_rooms` history used to
  gate the action buttons until the player has pressed `LOOK` in a room.
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

### 4.1 Decoding a line

The server speaks RFC 42TAP exclusively, so there is a single decode path:

```
parse_server_text()                    (network_parser.c)
 └─ for each complete line:
     ├─ rfc_route_line(line, ctx)       (rfc_parser.c)
     │    ├─ "OK …"   → route_ok()      key=value replies, JSON payloads, dialogue
     │    ├─ "ERR …"  → route_err()     maps the code to a readable log line
     │    └─ "EVT …"  → route_evt()     chat, presence, group, stats
     └─ anything unrecognised → printed verbatim in the Action Logs
```

`route_ok()` hands JSON bodies to `rfc_apply_json()` (`rfc_room.c`), which
dispatches on the payload shape: a `room` key goes to `rfc_apply_room()`, an
`attacker_hp` key to `rfc_apply_combat()`, a `quest_id` key to
`rfc_apply_quest()`, an `hp` key to `rfc_apply_status()`, and a bare array to
`rfc_apply_list()` — which is itself ambiguous between `INVENTORY` and
`QUESTS`, resolved with `ctx->last_cmd` and by testing whether the first
element is a string.

The verbatim fallback is deliberate: against another group's server, an
unknown line is shown rather than silently dropped.

> A previous revision of this client carried a second, ad-hoc decode path
> (`network_parser_utils.c`, `parse_player_state()`, `route_message()`, …) for
> a server that emitted `COMBAT:`, `DEAD:`, `Current Room : X` and similar
> non-RFC lines. The server no longer emits any of those, so that path was
> deleted — see `CORRECTIONS.md` §3.

Outgoing commands are always RFC form (`CHAT ROOM …`, `GROUP CREATE`, …),
sent from `chat_panel_utils.c` and `action_panel.c` only.

### 4.2 Avoiding log spam (`player_confirm.c`)

Some replies arrive far more often than their content changes: a `TALK` to a
quest giver triggers an automatic `INVENTORY` **and** `QUESTS` refresh, so a
naive implementation re-prints the same inventory and the same completed
quest on every single click.

`confirm_inventory()` and `quests_changed()` both build a short signature of
the payload, compare it to `ctx->last_inv_sig` / `ctx->last_quest_sig`, and
stay silent when nothing moved. `quests_changed()` is bypassed when
`ctx->last_cmd` is `QUESTS` — if the player deliberately pressed the QUEST LOG
button, the list is always shown, even unchanged.

`confirm_who()` simply records the server player count and echoes the raw
reply.

## 5. Module reference

| File | Responsibility |
|---|---|
| `main.c` | Entry point, argv parsing, init/loop/teardown, local `world.json` loading |
| `network.c` | Socket connect/send/receive, line-buffering, disconnect detection |
| `network_parser.c` | Line splitting, display-name resolution, `exits` object parsing |
| `rfc_parser.c` / `rfc_room.c` / `rfc_payload.c` / `rfc_events.c` | RFC 42TAP decoding and the follow-up refreshes it triggers |
| `player_confirm.c` | Change-detection so unchanged state is not re-logged |
| `layout.c` | HUD quadrant layout, top-level per-frame render call |
| `room_view.c` | Room background, NPC dialogue overlay, unexplored-room dimming, player counters |
| `room_entities.c` / `room_entities_utils.c` | NPC & room-item sprites, click-to-select, missing-texture outline |
| `map.c` | Fixed-layout minimap of the 8 rooms |
| `status_panel.c` | HP bar, clickable inventory list |
| `action_panel.c` / `action_panel_utils.c` / `action_panel_npc.c` | The 18 command buttons, enablement rules, NPC target bookkeeping |
| `chat_panel.c` / `chat_panel_utils.c` | Room/Global/Group tabs, text input, `/group` sub-commands |
| `graphics.c` / `graphics_items.c` / `graphics_npcs.c` | Texture preloading per room/item/NPC id |

## 6. Two rules that surprise newcomers

**Buttons are gated on `LOOK`.** `is_btn_active()` (`action_panel_utils.c`)
greys out every room-dependent button until the current room name appears in
`player.known_rooms`, which only `remember_room()` — called when the player
clicks `LOOK` — adds to. Moving into a room auto-refreshes the view but does
*not* register it, so the buttons stay grey until `LOOK` is pressed. This is
the single most common "the client is broken" report.

**An NPC with no sprite is drawn as an outline.** `draw_npc_sprite()` falls
back to a yellow rectangle outline (no fill) positioned by the ratio table in
`room_entities_utils.c` — that is how the Core Mainframe is represented, as a
frame around the reactor column of the AI Core artwork. Those ratios are tuned
against the default 1280×720 window; the room background is letterboxed to fit
its panel, so heavy resizing drifts the alignment.

## 7. Extending the client

**Adding a new RFC response format:** add a branch in `rfc_apply_json()`
(`rfc_room.c`) if it is a new JSON payload shape, or a new `else if` in
`route_ok()` / `route_evt()` (`rfc_parser.c`) if it is a `key=value` reply or
an event. Keep the 5-function/25-line caps in mind — split into a new
`rfc_*.c` file (and add it to the root `Makefile`'s `GUI_SRC`) if a file is
already full.

**Adding a button:** append to the `commands[]` and `labels[]` arrays in
`action_panel.c`, then update every id range in `handle_click()`,
`is_btn_active()` and `apply_npc_side_effects()` — the ids are positional, so
inserting in the middle shifts everything after it. The grid geometry in
`get_btn_rect()` assumes three columns; the row-height divisor must be raised
when a new row is needed.

**Adding a UI element:** panels are self-contained `render_*(bounds, ctx, …)`
functions called from `render_main_layout()`; they read `ctx` but never talk
to the network directly — outgoing commands go through `net_send_command()`
in `action_panel.c` / `chat_panel_utils.c` only.

## 8. Build & run

See the root `Makefile`: `make install-deps && make client_gui`, then
`make run-client-gui` (accepts `NAME=`, `IP=` and `PORT=`). No build step is
specific to this directory beyond what is already wired into `GUI_SRC`.

## 9. Known limitations

- The room-player counter updates on `LOOK` and on `EVT ROOM PRESENCE`, never
  by polling — a deliberate choice, since periodic polling was removed once it
  was found to flood the server.
- Item and NPC display names, sprites and the quest-giver test are resolved
  from the locally loaded `data/world.json`, not from the server. Against
  another group's server, names fall back to the raw id and sprites are
  missing; the client stays functional but looks plainer.
- The NPC placement ratios in `room_entities_utils.c` are calibrated for the
  default window size (see §6).
