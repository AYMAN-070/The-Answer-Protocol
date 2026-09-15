*This project has been created as part of the 42 curriculum by ayhammou, qdecross.*

# The Answer Protocol (TAP)

## Description
The Answer Protocol (TAP) is a small multiplayer text adventure. The project consists of a TCP server and two clients: a Command Line Interface (CLI) and a Graphical User Interface (GUI). The game provides a shared, persistent-feeling world where multiple players can explore rooms, chat, and cooperate in real time. 

## Architecture
This section explains our server design choices.
* The server is written entirely in C and relies on a non-blocking TCP concurrency model using `select()` multiplexing. 
* Instead of a complex threaded dispatcher, we use an inline command routing system (`process_command`). This ensures strict compliance with the 42 Norminette (max 25 lines, max 4 parameters) while maintaining high performance.
* Dynamic world states (like dropped items) are managed in a static memory array (`t_drop`) to avoid heavy real-time file I/O operations.

## Protocol Implementation
Our server implements all commands and events from the protocol.
* The server handles malformed commands with protocol-compliant error responses.
* **Deviation:** The official protocol suggests using `CHAT GLOBAL <msg>` for communication. To alleviate server-side network parsing and drastically improve the GUI client's rendering speed, we deviated from this specification by implementing `GCHAT <msg>` (Global) and `RCHAT <msg>` (Room). This deviation is documented here as required.

## Combat System
Our turn-based combat mechanics are built to be simple but punishing.
* Players start with 100 health points (HP).
* The `ATTACK` command deals damage to enemy NPCs based on a fixed formula, and NPCs execute a counter-attack based on their predefined difficulty.
* Additional combat commands like `DEFEND` (reduces incoming damage for one turn) and `FLEE` (allows escape to a random adjacent room) are fully implemented.
* **Death Penalty:** When a player's HP reaches 0, they respawn at a safe location with reduced health. We set this respawn health to 50 HP, and penalize the player by completely emptying their inventory. Combat results are logged and broadcasted to relevant players.

## Quest System
Our project implements at least 2 simple quests.
* **Progression & Validation:** Quests are triggered by the `TALK` command with specific NPCs. Validation is handled server-side by checking the player's inventory for required items (e.g., fetch quests).
* **Reward System:** Upon completion, the server removes the requested item from the player's inventory and grants a unique key-item necessary to access the final boss room.

## World Design
The world is loaded from a static JSON data file and features a coherent map.
* **Rooms:** The map contains at least 8 interconnected rooms forming a loop, allowing a full circuit without being a "line-only" map.
* **NPCs & Items:** The world includes at least 3 distinct NPC roles (dialogue, quest-giver, enemy) and at least 4 distinct items, with at least 2 being obtainable in-world. 
* Multi-word item names are fully supported (e.g., `energy cell`). Dropping an item in a room makes it available for other players to pick up.

## Server Logging
We implemented comprehensive server logging to monitor server behavior.
* All logs use a structured format containing precise timestamps, IP addresses, and log levels (INFO, WARN, ERROR).
* The server logs all client connections, disconnections, received commands, server responses, world state changes, and quest progress.
* **Abuse Monitoring:** The server actively monitors potential abuse patterns like command flooding. If a client sends more than 5 commands per second, the server ignores the inputs and issues a `WARN` log to protect the main thread.

## Group Contributions
* **ayhammou (Server & CLI):** Responsible for the core TCP server architecture, multiplexing, command parsing, and the CLI client implementation.
* **qdecross (GUI & World Design):** Responsible for the graphical client interface, visual parsers, and the design of the JSON world data (rooms, quests, NPCs).

## Instructions (Building and Running)
The repository includes a Makefile with the necessary targets to compile and run the project.

### 1. Install Dependencies & Compile
```bash
make install-deps
make

### 2. Run the Server
```bash
make run-server
```

### 3. Run the Clients
```bash
make run-client       # Launches the CLI client
make run-client-gui   # Launches the GUI client
```

### 4. Code Quality
```bash
make lint             # Checks the code against standard linting practices
make clean            # Cleans object files
```

## Testing
This section explains how to test the core features.
* **Multiplayer:** Launch the server, then open multiple terminal windows and run `make run-client` in each. Use `GCHAT` to verify broadcast messaging.
* **Combat & Quests:** Navigate to the guard NPC and use `TALK guard`. Pick up the required item using `TAKE` and return to the NPC to observe the quest completion logic. Engage an enemy with `ATTACK` to test the HP reduction and death/respawn mechanics.

## Resources
* **References:** Beej's Guide to Network Programming (TCP/IP sockets).
* **AI Usage:** Generative AI (LLMs) was used strictly to assist with C syntax optimization (reducing function lengths to under 25 lines and limiting parameters to 4) and debugging complex network behavior (understanding TCP Streams and blocking sockets). AI was also used to help structure this README document to ensure compliance with the project specifications. All AI-generated logic was peer-reviewed and manually integrated.