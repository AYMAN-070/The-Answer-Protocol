# Corrections serveur — mise en conformité RFC 42TAP / sujet

Date : 2026-09-20
Auteur des corrections : qdecross
Périmètre : `server/` uniquement (le GUI a été corrigé séparément, le CLI n'a pas été touché).

Ce fichier documente **exactement** ce qui a été modifié et pourquoi. La section
« Déviations protocole » est à recopier dans le README, le sujet exigeant que toute
déviation y soit documentée et justifiée.

---

## 1. Conformité RFC 42TAP

| Commande / événement | Avant | Après | Référence |
|---|---|---|---|
| `MOVE <dir>` | renvoyait la réponse complète de `LOOK` | `OK room=<room-id>` | RFC 5.1.3 |
| `MOVE` direction absente ou invalide | `ERR missing direction` / `ERR invalid direction` (sans code) | `ERR 301 NO_EXIT` | RFC 5.1.3 / 8.2 |
| `GROUP JOIN <arg>` | attendait un **identifiant numérique** (`atoi`) | attend le **nom du leader**, le joueur rejoint le groupe de ce leader | RFC 5.3.3 |
| `GROUP JOIN` sur un joueur sans groupe | rejoignait le groupe `0` | `ERR 401 NOT_IN_GROUP` | RFC 8.2 |
| `GROUP` joueur cible inconnu | `ERR player not found` (sans code) | `ERR 410 PLAYER_NOT_FOUND` | RFC 4.1 (ABNF : code sur 3 chiffres) |
| `DROP` objet absent de l'inventaire | `ERR item not found` (sans code) | `ERR 404 ITEM_NOT_IN_INVENTORY` | RFC 5.4.2 |
| `DEFEND` | `COMBAT:You defend! ...` (hors ABNF) | `OK {"attacker_hp":…, "target_hp":…, "damage":…, "status":"defend"}` | RFC 4.1 |
| `FLEE` réussi | `EVENT:Flee successful!` + `LOOK` (hors ABNF) | `OK room=<room-id>` + `EVT ROOM PRESENCE LEAVE/ENTER` | RFC 4.1 / 6.2.1 |
| `FLEE` échoué | `COMBAT:Flee failed! ...` (hors ABNF) | `OK {…, "status":"flee_failed"}` | RFC 4.1 |
| Mort du joueur | `DEAD:You died! ...` (hors ABNF) | `EVT PLAYER DEATH respawn=medbay` | RFC 4.1 (extension, cf. §3) |
| `CHAT <scope>` avec scope invalide | **aucune réponse** | `ERR 400 BAD_REQUEST` | RFC 9.3 |
| `CHAT GROUP` sans groupe | diffusait à tous les joueurs sans groupe (`group_id == 0`) | `ERR 401 NOT_IN_GROUP` | RFC 8.2 |
| `EVT GROUP JOIN <user>` | jamais émis | émis aux membres du groupe lors d'un `GROUP JOIN` | RFC 6.2.3 |
| `EVT GROUP LEAVE <user>` | jamais émis | émis aux membres du groupe lors d'un `GROUP LEAVE` | RFC 6.2.3 |
| `ATTACK` sur PNJ déjà mort | `ERR 406 NPC_ALREADY_DEAD` (406 = `NO_QUEST_AVAILABLE` dans la RFC) | `ERR 409 NPC_ALREADY_DEAD` | RFC 8.2 |
| Commande inconnue / argument manquant | `ERR unknown command`, `ERR missing name`, `ERR missing item`… (sans code) | `ERR 400 BAD_REQUEST` (ou code 404 spécifique) | RFC 4.1 |
| Anti-flood | `ERR ignored` (sans code) | `ERR 429 TOO_MANY_REQUESTS` | RFC 4.1 |

## 2. Conformité sujet (TAP v1.4)

| Exigence | Avant | Après |
|---|---|---|
| « Items can be referenced by ID or display name » + « Multi-word item names are fully supported » | comparaison `strcmp` exacte sur l'id uniquement | `resolve_id()` traduit un nom d'affichage en id avant traitement, pour `TAKE`, `DROP`, `TALK`, `ATTACK`, `DEFEND`, `QUEST`. `TAKE Energy Cell` → `OK taken=energy_cell` |
| « Validates that all exits and references in the world are correct » | aucune validation | `validate_world()` au démarrage : chaque sortie de chaque salle doit pointer vers une salle existante, sinon log `ERROR` + arrêt du serveur |
| « remove player state before broadcasting leave events » | aucun `EVT ROOM PRESENCE LEAVE` à la déconnexion | émis après mise à zéro de l'état, sur `QUIT` **et** sur déconnexion brutale (`read() <= 0`) |
| « STATUS shows current player health and combat status » | `"status": "healthy"` en dur | `"healthy"` / `"wounded"` (hp < max/2) / `"in_combat"`, via le champ `in_combat` désormais réellement géré |
| « Log every command received with player name and parameters » | seul le mot-clé était loggé (`TALK`) | la ligne complète est loggée (`TALK sec_drone`) |
| « Log quest progress and completion events » | aucun log de quête | logs sur quête proposée, démarrée et complétée |
| « Log world state changes (… combat results) » | aucun log de combat | log à chaque round de combat, sur `ATTACK`, `DEFEND`, `FLEE` et mort |
| « Use structured logging format » | deux `printf` de debug polluaient stdout | supprimés (`server_cmd.c`, `server_cmd_look.c`) |

## 3. Déviations protocole — **à recopier dans le README**

La RFC impose `error-code = 3DIGIT` (§4.1) mais ne définit pas de code pour tous les
cas rencontrés. Plutôt que de renvoyer des erreurs sans code (non conformes à l'ABNF),
les codes suivants ont été ajoutés. Ils respectent la sémantique 4xx de la RFC §7.3
(erreur non fatale, la connexion continue) :

| Code | Message | Sens |
|---|---|---|
| `400` | `BAD_REQUEST` | commande inconnue, argument manquant ou scope de chat invalide |
| `403` | `ACCESS_DENIED` | salle verrouillée (`ai_core`, tant que personne n'y est entré avec le `red_keycard` — cf. §6) |
| `407` | `ITEM_NOT_OBTAINABLE` | objet présent mais `obtainable: false` (décor fixe) |
| `408` | `INVENTORY_FULL` | inventaire plein (5 emplacements) |
| `409` | `NPC_ALREADY_DEAD` | cible déjà vaincue |
| `410` | `PLAYER_NOT_FOUND` | joueur cible d'un `GROUP INVITE` / `GROUP JOIN` inconnu |
| `429` | `TOO_MANY_REQUESTS` | anti-flood (plus de 5 commandes par seconde) |

Événement ajouté, absent de la RFC §6.2 :

| Événement | Sens |
|---|---|
| `EVT PLAYER DEATH respawn=<room-id>` | le joueur est mort et réapparaît dans la salle indiquée |

Justification : la RFC ne prévoit aucun événement de mort alors que le sujet impose un
respawn. Le format suit la grammaire `EVT <category> <type> <data>` de la §7.1.

## 4. Fichiers et fonctions ajoutés

| Fichier | Contenu | Raison |
|---|---|---|
| `server/src/server_resolve.c` (nouveau) | `resolve_id()` + `name_matches()` | traduction nom d'affichage → id. Parsing `strstr`/`strchr`, cohérent avec `server_json.c`. Ajouté au `Makefile` (`SERVER_SRC` et règle `$(SERVER)`) |
| `server_world.c` | `validate_world()`, `check_room_exits()`, `room_exists()` | validation du monde au chargement |
| `server_utils.c` | `broadcast_room()`, `broadcast_group()` | `broadcast_room` était `static` dans `server_cmd_move.c` ; il est désormais partagé (move, flee, quit, déconnexion). `broadcast_group` est son équivalent pour les groupes |
| `server_cmd_talk.c` | `handle_disconnect()`, `complete_repair_quest()` | factorisation imposée par la limite de 25 lignes de la norme |
| `server_cmd_attack.c` | `check_attack_err()` | même modèle que le `check_take_err()` existant |
| `server_cmd_take_inventory.c` | `take_item_slot()` | idem |
| `server_group_who.c` | `group_join()`, `group_leave()` | idem |
| `server_cmd_chat.c` | `chat_broadcast()` | idem |
| `server_quests.c` | `send_quest()` | une quête déjà active se re-annonçait `"available"` ; elle renvoie maintenant `"active"` |
| `server_world.c` | `world_state(mode, key)` | état de monde partagé (cf. §6). Fonction à état statique avec un paramètre `mode`, sur le modèle de `manage_taken()` et `manage_dropped()` |
| `server_quests.c` | `reward_override()`, `append_quest()` | récompense du kill de l'`alpha_drone` et construction d'une entrée de `QUESTS` ; sortent `handle_attack` et `handle_quests` de la limite de 25 lignes |
| `server_cmd_move.c` | `ai_core_locked()` | verrou de l'`ai_core`, sorti de `handle_move` pour la même raison |

Signature modifiée : `handle_drop(t_player *, char *, char *)` — le paramètre
`t_player *c` est devenu inutile, `DROP` ne renvoyant plus qu'une seule réponse.

## 5. Impact sur les clients

- **GUI** : compatible sans modification. `OK room=` déclenche déjà un `LOOK`, les JSON
  de `DEFEND`/`FLEE` passent par `rfc_apply_combat`, et `EVT PLAYER DEATH` s'affiche
  dans le panneau de logs, les HP étant remis à jour par le `STATUS` qui suit.
- **Conséquence** : le parseur historique du GUI (`COMBAT:`, `EVENT:`, `DEAD:`, `NPC:`,
  `DROPPED:` dans `gui/src/network_parser_utils.c`) ne reçoit plus rien du serveur. Il
  peut être supprimé, ou conservé comme filet de sécurité pour un serveur d'un autre
  groupe.
- **CLI** : affiche le texte brut, aucun impact.
- **`DROP`** ne renvoie plus qu'une seule réponse (avant : `OK dropped=` + `INVENTORY`
  + `LOOK`, soit trois réponses pour une commande, ce qui empêche un client d'apparier
  réponse et requête).


## 6. Conception du monde partagé — **à recopier dans le README**

Le sujet impose que les objets soient des instances uniques (*« Items exist as unique
instances in the world / no duplication »*) et laisse explicitement la mécanique de
progression des quêtes au choix du groupe, à documenter. Le choix retenu est celui d'une
**run partagée unique** : la station a un seul ingénieur blessé et un seul drone alpha,
donc les quêtes sont un état **du monde**, pas du joueur.

| Règle | Comportement |
|---|---|
| Quête déjà résolue par quelqu'un | `QUEST <pnj>` renvoie `ERR 406 NO_QUEST_AVAILABLE` aux joueurs suivants, conformément à la RFC §8.2 : *« NPC has no quests or quest already completed »* |
| Dialogue après résolution | L'ingénieur répond « System restored. Thanks! » et le terminal « Lockdown lifted. » à **tout le monde**, plus seulement à celui qui a agi |
| `QUESTS` d'un joueur ayant pris la quête avant sa résolution | l'entrée passe à `completed`, au lieu de rester bloquée en `active 0/1` sur un objectif devenu impossible |
| Porte de l'`ai_core` | le `red_keycard` est exigé pour la **première** ouverture ; ensuite la salle reste ouverte à tous. Cohérent avec le dialogue du `core_mainframe` : *« Lockdown lifted. You are authorized to access the escape shuttles. »* Le confinement est un état de la station, pas un privilège personnel |
| Mort d'un joueur | l'inventaire **tombe au sol** dans la salle où il est mort, au lieu d'être détruit. Récupérable par lui-même ou par un autre joueur |

Sans la dernière règle, un joueur mourant en portant le `red_keycard` détruisait
définitivement le seul exemplaire existant — le drone étant mort, aucun autre ne pouvait
être obtenu, et l'`ai_core` devenait inaccessible à tous jusqu'au redémarrage du serveur.
Le partage d'objets entre joueurs passe donc par le `DROP` ou par la mort, jamais par la
duplication.

Les trois drapeaux (`repair_done`, `override_done`, `ai_core_open`) vivent dans
`world_state()` et ne sont pas persistés : redémarrer le serveur réinitialise le monde,
ce que le sujet autorise explicitement (*« No persistence is required; state may reset
when the server restarts »*).
