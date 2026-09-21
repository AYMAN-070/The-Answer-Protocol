# Prometheus Station — Player's Guide

A tutorial for the graphical client and a full walkthrough of the game.
This document is separate from `README.md`, which remains the technical documentation.

---

## 1. Starting a game

```bash
make                       # builds the server and both clients
make run-server            # in a first terminal
make run-client-gui        # in a second one
```

To play with several people, every client needs a **different name**:

```bash
make run-client-gui NAME=alice
make run-client-gui NAME=bob
```

Without a name everyone connects as `gui_user`, and the server rejects the second one
with "Name already in use". To reach a server on another machine:
`make run-client-gui NAME=alice IP=192.168.1.20`.

## 2. The screen

| Area | Contents |
|---|---|
| **Room View** (large left panel) | the room artwork, items lying on the floor in the top-left corner, the NPCs, and at the bottom the room name and description — or the dialogue of the NPC you are talking to |
| **Inventory & Status** (right) | your health bar, your inventory, and below them the **Command Matrix**, the button grid |
| **Chat** (bottom left) | three channels, press `TAB` to cycle through them |
| **Action Logs** (bottom right) | server replies, errors, and events happening in your room |

Two counters are shown in the room view: how many players are **in the room**, and how
many are **on the server**.

## 3. The rule that blocks everyone at first

**When you enter a new room, most buttons are greyed out.** You must click **LOOK**
once for the room to be registered; movement and interaction then become available.

The habit to build: **click LOOK every time you change room.**

Only `INVENTORY`, `STATUS`, `QUEST LOG`, `WHO`, `GROUP` and `QUIT` stay usable
everywhere, since they do not depend on the room.

## 4. Picking a target

Three groups of buttons act on a target you must **click to select**:

- **an item on the floor** → click its tile at the top of the room view, then `TAKE`
- **an item in your inventory** → click its row in the Inventory panel, then `DROP`
- **an NPC** → click its sprite in the room, then `TALK`, `QUEST`, `ATTACK` or `DEFEND`

The selected target is highlighted in yellow. With nothing selected, the first one in
the list is used.

## 5. The buttons, one by one

### Movement and observation

| Button | Effect |
|---|---|
| **LOOK** | refreshes the room **and registers it** — essential on arrival |
| **NORTH / WEST / EAST / SOUTH** | move. A greyed-out button means there is no exit that way |

### Items

| Button | Effect |
|---|---|
| **TAKE** | picks up the selected item on the floor. Greyed out if the room is empty |
| **DROP** | puts the selected inventory item down. Other players can then pick it up |
| **INVENTORY** | refreshes your item list (5 slots maximum) |

### NPCs

| Button | Effect |
|---|---|
| **TALK** | talks to the selected NPC; the reply appears at the bottom of the room view. This is also how you **hand in** a quest item |
| **QUEST** | asks the NPC for a quest. "No quest available" means it has none, or that the quest is already solved |
| **QUEST LOG** | lists your quests and their status |

### Combat

| Button | Effect |
|---|---|
| **ATTACK** | strikes the selected NPC: 15-25 damage bare-handed, **30-40 with the Laser Pistol**. If it survives, it strikes back for 10-20. On a non-hostile target: "This target cannot be attacked" |
| **DEFEND** | raises your guard. It costs nothing and deals nothing: the counter-attack of **your next strike** is halved (5-10 instead of 10-20). The guard drops after that strike, on death, or when you change room |
| **FLEE** | a one-in-two chance of escaping to the previous room; otherwise you take 10-20 |
| **STATUS** | your HP and state: `healthy`, `wounded` below half HP, or `in_combat` |

The optimal combat loop is therefore **DEFEND → ATTACK → DEFEND → ATTACK**. It takes
twice as many clicks to kill a target, but you take half the damage.

**If you die**: you respawn in the Medbay with 50 HP, and **your inventory drops on the
floor in the room where you fell**. Nothing is destroyed — go back and collect it, but
another player may beat you to it.

### Multiplayer

| Button | Effect |
|---|---|
| **WHO** | how many players are connected |
| **GROUP** | creates a group |
| **QUIT** | disconnects cleanly |

The chat panel has three channels, cycled with `TAB`: **Room** (your room), **Global**
(the whole server), **Group** (your group). Type, then press `Enter`.

In the **Group** channel, four commands can be typed directly:

| Command | Effect |
|---|---|
| `/create` | create a group |
| `/invite <player>` | invite someone |
| `/join <leader>` | join a player's group, **by their name** |
| `/leave` | leave your group |

## 6. The station

Each arrow is labelled with the direction you travel. Every exit has a matching return
exit, so you can always retrace your steps.

```
   medbay ───── east ─────► cafeteria
   Emergency Medkit          Cleaning Bot
   Injured Engineer               │
     ▲                            │ south
     │ north                      ▼
   armory                    crew_quarters
   (empty)                    Energy Cell
     ▲                        Security Drone
     │ north                       │
     │                             │ west
   main_corridor ◄─────────────────┘
   Security Drone
     │        │
     │        └─── south ───► airlock  (start)
     │
     └─── west ───► maintenance_bay ─── west ───► ai_core  (locked)
                    Alpha Drone                   Core Mainframe
                    AI Terminal
                    Central Generator
```

Five rooms form a loop: `main_corridor → armory → medbay → cafeteria → crew_quarters →
main_corridor`. The `airlock` and the `ai_core` are the two dead ends.

Do not try to picture the station as a floor plan: going *north, north, east, south,
west* brings you back to where you started, which no real building allows. The map is a
network of connections, not a coherent geometry — follow the directions, not your sense
of space.

Only the **Security Drone** and the **Alpha Drone** are hostile. The other NPCs cannot
be attacked.

## 7. Full walkthrough

Click **LOOK** on arriving in each room — this is not repeated below.

**1. Airlock** — your starting point. A single exit.
→ `NORTH`

**2. Main Corridor** — a Security Drone (30 HP) patrols here. You can ignore it, or take
it down in two hits for practice.
→ `EAST`

**3. Crew Quarters** — the **Energy Cell** lies on the floor. Click its tile, then
`TAKE`. It is the first quest item, so hold on to it.
→ `NORTH`

**4. Cafeteria** — a harmless Cleaning Bot. Nothing to do here.
→ `WEST`

**5. Medbay** — the **Injured Engineer** is waiting. Click him, then:
- `QUEST` to register the **power_up** quest
- `TALK`: since you are carrying the Energy Cell, he trades it for the **Laser Pistol**

The pistol doubles your damage and is essential for what follows. The **Emergency
Medkit** on the floor is a collectible with no effect; take it if you want.
→ `SOUTH`

**6. Armory** — the weapon racks have been stripped; the room is empty. A shortcut only.
→ `SOUTH`, then `WEST`

**7. Maintenance Bay** — two NPCs. Click the **AI Terminal**, then `QUEST` to register
the **system_override** quest.

Now click the **Alpha Drone** (80 HP) and alternate **DEFEND → ATTACK**, three or four
times. With the Laser Pistol you deal 30-40 per hit while taking only 5-10 thanks to the
guard. Destroying it grants the **Red Keycard** and lifts the station lockdown.

If your HP drops too low, `FLEE` takes you back to the Main Corridor — but only half the
time.
→ `WEST`

**8. AI Core** — the door requires the Red Keycard the first time. Once it has been
opened, **it stays open for every player**, permanently.

Click the **Core Mainframe** and press `TALK` twice — every NPC alternates between its
two lines, so the second press delivers the closing one:

> *"Security protocols restored."*
> *"Lockdown lifted. You are authorized to access the escape shuttles. Well done."*

**You have finished the game.**

## 8. What to know when playing together

The station is a **single shared run**: there is only one injured engineer and only one
Alpha Drone.

- The first player to solve a quest solves it **for everyone**. Later players get "No
  quest available", and the NPCs tell them the matter is already settled.
- Items are unique instances. There is only one Energy Cell, one Laser Pistol, one Red
  Keycard.
- To share, use `DROP`: the item becomes available to others in the room where you left
  it.
- Since the AI Core door stays open, it only takes **one** player reaching the end for
  everyone to be able to visit it.

Restarting the server resets the world completely: quests, collected items, defeated
NPCs and opened doors.

## 9. Common messages in the Action Logs

| Message | Meaning |
|---|---|
| `Name already in use` | another client already uses that name; relaunch with `NAME=` |
| `Not found: ...` | the target is not in this room |
| `This target cannot be attacked` | that NPC is not hostile |
| `No quest available` | that NPC has no quest, or it is already solved |
| `Error: 403 ACCESS_DENIED` | locked door: you need the Red Keycard |
| `Error: 409 NPC_ALREADY_DEAD` | that target has already been destroyed |
| `Error: 429 TOO_MANY_REQUESTS` | you are clicking too fast; the limit is 5 commands per second |
| `<player> left the room` | another player left your room — unrelated to whatever you just did |

