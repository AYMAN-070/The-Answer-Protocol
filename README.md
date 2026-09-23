*This project has been created as part of the 42 curriculum by ayhammou and qdecross.*

# The Answer Protocol (TAP)

## Description
The Answer Protocol (TAP) is a multiplayer text-based adventure game. The project consists of a TCP server and two clients: a Command Line Interface (CLI) and a Graphical User Interface (GUI). The game provides a shared, persistent-feeling world where multiple players can explore rooms, chat, and cooperate (or compete) in real time to escape a locked-down space station.

## Server Architecture & Technical Choices
This section explains our server design choices, handled primarily by **ayhammou**.
* **Mono-thread Multiplexing:** The server is written entirely in C and relies on a non-blocking TCP concurrency model using `select()`. Instead of a heavy multi-threaded approach, a single thread efficiently handles multiple clients, completely avoiding data races and mutex bottlenecks.
* **TCP Stream Safety:** TCP is a stream, not a message-based protocol. We implemented robust buffer management using `memmove()` and pointer arithmetic to accurately slice incomplete or grouped commands sent by clients.
* **Ghost Data Prevention:** We ensure strict memory safety. Upon a client's disconnection (whether a clean `QUIT` or a brutal socket crash), their session slot is entirely wiped using `memset()` to prevent "ghost data" (lingering HP, quests, or group IDs) from affecting the next player taking that slot.
* **Crash-Proofing:** Server signals (`SIGPIPE`) are ignored to prevent the server from crashing when writing to unexpectedly closed sockets. Standard output (`stdout`) is unbuffered (`setvbuf`) to ensure real-time logs even in case of a fatal error.

## World Design & Graphical Interface (Raylib)
Designed by **qdecross**, the game world is not just a text-based JSON file; it is fully brought to life through a custom 2D Graphical User Interface built from scratch using the **Raylib** library.

* **Raylib Engine Integration:** The GUI client uses Raylib to manage the rendering loop, texture loading, custom fonts, and mouse interactions. The visual loop runs smoothly alongside the non-blocking network socket listener, ensuring real-time server synchronization without freezing the graphics.
* **Visual Rooms & NPCs:** Every room defined in the server's world data was carefully crafted with specific background artworks. NPCs (like the Security Drone or Injured Engineer) and items are dynamically rendered as 2D sprites in the environment. Their presence on screen is entirely dictated by the server's real-time state.
* **Dynamic Command Matrix:** Instead of typing commands, the Raylib interface features a clickable UI grid. Clicking on a 2D NPC or item sprite directly in the room highlights it, and clicking action buttons (`TALK`, `TAKE`, `ATTACK`) automatically generates and sends the correct formatted protocol string to the server.
* **Shared Ecosystem (Game Design):** To respect the project's "unique instances" rule, the world logic is a **Shared Run**. Quests are first-come, first-served (using our `world_state()` engine). When a player drops an inventory item, its sprite instantly appears on the floor of the room's UI for any other player to visually see and pick up.

## Combat System
Our turn-based combat mechanics are built to be simple but punishing.
* Players start with 100 health points (HP).
* **ATTACK:** Deals 15-25 damage (30-40 with the Laser Pistol). If the NPC survives, it automatically counter-attacks (10-20 damage).
* **DEFEND:** Raises a guard state for the player's *next* attack, halving the incoming counter-attack damage. Optimal combat loop: `DEFEND` → `ATTACK`.
* **FLEE:** Allows a 50% chance to escape to an adjacent room without taking damage.
* **Death Penalty:** When a player's HP reaches 0, they respawn in the Medbay with 50 HP. Their entire inventory drops on the floor. Combat results are broadcasted to everyone in the room.

## Quest System
Our quest progression mechanics are based on a "Shared World" philosophy. Quests act as global server states.
* **Progression & Validation:** Quests are triggered using the `TALK` command with specific NPCs (like the `injured_engineer` or `ai_terminal`). Validation happens server-side by checking the player's inventory for required items (e.g., `Energy Cell`) or by verifying the defeat of specific enemies (e.g., `alpha_drone`).
* **Reward System:** Upon successful completion, the server removes the requested item from the player's inventory and grants a unique key-item (e.g., `laser_pistol`, `red_keycard`) necessary to progress or access the final boss room. Since quests are global, subsequent players asking for the quest will receive an `ERR 406 NO_QUEST_AVAILABLE` and a unique dialogue indicating the task is already done.

## World Design
The world is loaded from a static JSON data file and features a coherent, interconnected map.
* **Layout & Room Connections:** The map contains at least 8 interconnected rooms forming a continuous loop (e.g., `main_corridor` -> `armory` -> `medbay` -> `cafeteria` -> `crew_quarters` -> `main_corridor`), ensuring players can make a full circuit without being trapped in a "line-only" map. Rooms like the `airlock` and `ai_core` serve as optional branches or dead-ends.
* **NPC Roles:** The station is populated by at least 3 distinct NPC roles: dialogue-only NPCs (e.g., `Cleaning Bot`), quest-givers (e.g., `Injured Engineer`, `AI Terminal`), and enemies (e.g., `Security Drone`, `Alpha Drone`).
* **Item Distribution:** The world includes at least 4 distinct items, with unique instances initially placed in specific rooms (e.g., `Energy Cell`, `Emergency Medkit`). Dropping an item makes it instantly available for other players in that room to pick up, reinforcing the shared economy.

## Testing
This section explains how to test the core features of the game.
* **Multiplayer Functionality:** Launch the server, then open multiple terminal windows and run `make run-client` (or `make run-client-gui NAME=player2`) in each. Use the `WHO` command to see connected players, and test `CHAT GLOBAL` or `CHAT ROOM` to verify broadcast messaging.
* **Quest Mechanics:** Navigate to the `medbay` and use `TALK injured_engineer`. Locate the `Energy Cell` in the `crew_quarters`, pick it up using `TAKE`, and return to the NPC to observe the global quest completion logic and receive your reward.
* **Combat System:** Find a hostile NPC (e.g., `Security Drone` in the `main_corridor`) and engage it with `ATTACK`. Test the damage reduction by using `DEFEND` before an attack. To test the death penalty, allow your HP to drop to 0 to verify the respawn in the `medbay` and confirm that your inventory drops in the combat room.

## Protocol Implementation
Our server strictly implements the TAP RFC, with necessary deviations and additions documented below to improve the multiplayer and GUI experience.

**Deviations & Additions:**
1. **Chat Scopes:** The official protocol suggests `CHAT GLOBAL <msg>`. To alleviate server-side network parsing and drastically improve GUI rendering speed, we implemented `CHAT GLOBAL <msg>` (Global) and `CHAT ROOM <msg>` (Room).
2. **Custom Error Codes (4xx/2xx):** The RFC requires a `3DIGIT` error code but lacks specific codes for many edge cases. We added standard HTTP-like codes to maintain ABNF compliance:
   * `202 NOT_AUTHENTICATED`: Issued if commands are sent before `CONNECT`.
   * `400 BAD_REQUEST`: Unknown command or missing arguments.
   * `403 ACCESS_DENIED`: Locked doors (e.g., `ai_core` without the keycard).
   * `404 ROOM_NOT_FOUND`: Room ID mapping error.
   * `407 ITEM_NOT_OBTAINABLE`: Trying to take background scenery.
   * `408 INVENTORY_FULL`: Maximum 5 slots reached.
   * `409 NPC_ALREADY_DEAD`: Target is already defeated.
   * `410 PLAYER_NOT_FOUND`: Target for `GROUP JOIN/INVITE` does not exist.
   * `429 TOO_MANY_REQUESTS`: Anti-flood protection triggered (more than 5 cmds/sec).
3. **Custom Events:** Added `EVT PLAYER DEATH respawn=<room-id>` to notify clients of respawns, and `EVT ROOM COMBAT <player> damage=<d> target_hp=<hp>` to broadcast fight actions to everyone in the same room.

## Graphical User Interface (GUI)
The GUI client provides a fully immersive layout:
* **Room View:** Displays room artwork, floor items, NPCs, and dynamic dialogues.
* **Status Panel:** Visual health bar, inventory slots, and the Command Matrix (clickable buttons for actions).
* **Chat & Logs:** Multi-channel chat (`TAB` to cycle between Room, Global, Group) and real-time Action Logs for server events.
* *Note: To connect multiple GUI instances locally, use `make run-client-gui NAME=player_name`.*

## Server Logging
We implemented comprehensive, JSON-escaped server logging to monitor server behavior.
* All logs use a structured format containing precise timestamps, IP addresses, and log levels (`INFO`, `WARN`, `ERROR`).
* The server logs all client connections, received commands, server responses, world state changes, combat rounds, and quest progress.

## Group Contributions
* **ayhammou (Server & CLI):** Core TCP server architecture, `select()` multiplexing, socket memory safety, strict TCP stream buffering (`memmove`), Norminette compliance, and CLI client implementation.
* **qdecross (GUI & World Design):** Graphical client interface development, visual parsing, RFC compliance audits, Shared World state mechanics (`world_state`), JSON data mapping, and game design.

## Instructions (Building and Running)

### 1. Install Dependencies & Compile
make install-deps\
make

### 2. Run the Server
make run-server

### 3. Run the Clients
make run-client       # Launches the CLI client\
make run-client-gui   # Launches the GUI client

### 3. Code Quality
make lint             # Checks the code against standard linting practices\
make clean            # Cleans object files

## Resources
References: Beej's Guide to Network Programming (TCP/IP sockets).

AI Usage: Generative AI was used strictly to assist with C syntax optimization (Norminette formatting) and debugging complex network edge-cases (TCP Stream chunking, memmove logic, blocking sockets). AI also assisted in formatting this README. All AI-generated logic was peer-reviewed, deeply understood, and manually integrated.