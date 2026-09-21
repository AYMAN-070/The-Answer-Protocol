# Corrections serveur — mise en conformité RFC 42TAP / sujet

Date : 2026-09-20
Auteur des corrections : qdecross
Périmètre : `server/` uniquement (le GUI a été corrigé séparément, le CLI n'a pas été touché).

Ce fichier documente **exactement** ce qui a été modifié et pourquoi. La section
« Déviations protocole » est à recopier dans le README, le sujet exigeant que toute
déviation y soit documentée et justifiée.

---

## 1. Conformité RFC 42TAP

| Commande / événement | Avant | Après |
|---|---|---|
| `MOVE <dir>` | renvoyait la réponse complète de `LOOK` | `OK room=<room-id>` |
| `MOVE` direction absente ou invalide | `ERR missing direction` / `ERR invalid direction` (sans code) | `ERR 301 NO_EXIT` |
| `GROUP JOIN <arg>` | attendait un **identifiant numérique** (`atoi`) | attend le **nom du leader**, le joueur rejoint le groupe de ce leader |
| `GROUP JOIN` sur un joueur sans groupe | rejoignait le groupe `0` | `ERR 401 NOT_IN_GROUP` |
| `GROUP` joueur cible inconnu | `ERR player not found` (sans code) | `ERR 410 PLAYER_NOT_FOUND` |
| `DROP` objet absent de l'inventaire | `ERR item not found` (sans code) | `ERR 404 ITEM_NOT_IN_INVENTORY` |
| `DEFEND` | `COMBAT:You defend! ...` (hors ABNF) | `OK {"attacker_hp":…, "target_hp":…, "damage":…, "status":"defend"}` |
| `FLEE` réussi | `EVENT:Flee successful!` + `LOOK` (hors ABNF) | `OK room=<room-id>` + `EVT ROOM PRESENCE LEAVE/ENTER` |
| `FLEE` échoué | `COMBAT:Flee failed! ...` (hors ABNF) | `OK {…, "status":"flee_failed"}` |
| Mort du joueur | `DEAD:You died! ...` (hors ABNF) | `EVT PLAYER DEATH respawn=medbay` |
| `CHAT <scope>` avec scope invalide | **aucune réponse** | `ERR 400 BAD_REQUEST` |
| `CHAT GROUP` sans groupe | diffusait à tous les joueurs sans groupe (`group_id == 0`) | `ERR 401 NOT_IN_GROUP` |
| `EVT GROUP JOIN <user>` | jamais émis | émis aux membres du groupe lors d'un `GROUP JOIN` |
| `EVT GROUP LEAVE <user>` | jamais émis | émis aux membres du groupe lors d'un `GROUP LEAVE` |
| `ATTACK` sur PNJ déjà mort | `ERR 406 NPC_ALREADY_DEAD` (406 = `NO_QUEST_AVAILABLE` dans la RFC) | `ERR 409 NPC_ALREADY_DEAD` |
| Commande inconnue / argument manquant | `ERR unknown command`, `ERR missing name`, `ERR missing item`… (sans code) | `ERR 400 BAD_REQUEST` (ou code 404 spécifique) |
| Anti-flood | `ERR ignored` (sans code) | `ERR 429 TOO_MANY_REQUESTS` |
| **Commandes avant `CONNECT`** | **toutes acceptées** : un client anonyme pouvait diffuser du chat à tout le serveur (`EVT GLOBAL CHAT  msg`, nom vide, événement malformé), `STATUS` renvoyait `hp: 0, max_hp: 0`, `LOOK`/`MOVE` renvoyaient des erreurs trompeuses | seules `CONNECT` passe ; tout le reste renvoie `ERR 202 NOT_AUTHENTICATED` |
| Commande illisible | confondue avec l'anti-flood (`ERR 429`) | `ERR 400 BAD_REQUEST`, distinct de l'anti-flood |
| `WHO` / `EVT STATS` | comptaient les sockets acceptées mais jamais authentifiées | ne comptent que les joueurs authentifiés |
| Caractères UTF-8 dans les logs | `José` était journalisé `Jos`, `été très` devenait `t trs` : le filtre de caractères de contrôle utilisait un `char` signé, donc tout octet UTF-8 (≥ 0x80, donc négatif) était supprimé | comparaison sur `unsigned char` : les caractères de contrôle restent filtrés, l'UTF-8 est préservé |

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
| « Log all server responses and error codes sent to clients » | aucune réponse ni aucun code d'erreur n'était loggé | les 55 réponses passent par `send_reply()`, qui journalise avant d'écrire : `INFO` pour un `OK`, `WARN` pour un `ERR` |
| « Use structured logging format (JSON) for easy parsing » | une réponse ou un message contenant un `"` cassait le JSON du log (`CHAT GLOBAL il a dit "bonjour"` produisait une ligne illisible) | `server_log()` échappe `"` et `\` et filtre les caractères de contrôle ; chaque ligne est du JSON valide, vérifié par parsing |
| « Combat results must be logged **and broadcast** to relevant players » | seul l'attaquant recevait le résultat | `EVT ROOM COMBAT <joueur> damage=<d> target_hp=<hp>` diffusé aux joueurs de la salle, et `EVT ROOM COMBAT <joueur> defeated` à la mort |
| Données de quête | le serveur renvoyait `repair` / `override`, absents de `data/world.json` qui déclare `power_up` / `system_override` | ids alignés sur le fichier de monde, et la récompense est désormais **lue** dans `data/world.json` via `get_quest_reward()` |
| « Broadcasts messages without interruption if a client disconnects mid-send » | aucune gestion de `SIGPIPE` : écrire dans la socket d'un client parti brutalement pouvait terminer le serveur. La garantie ne tenait qu'au fait que `select()` traite la déconnexion avant la diffusion — non déclenché en 12 tentatives de course, mais dépendant du timing | `signal(SIGPIPE, SIG_IGN)` au démarrage : une écriture vers une socket morte échoue désormais sans tuer le processus |

## 3. Déviations protocole — **à recopier dans le README**

La RFC impose `error-code = 3DIGIT` mais ne définit pas de code pour tous les
cas rencontrés. Plutôt que de renvoyer des erreurs sans code (non conformes à l'ABNF),
les codes suivants ont été ajoutés. Ils respectent la sémantique 4xx de la RFC
(erreur non fatale, la connexion continue) :

| Code | Message | Sens |
|---|---|---|
| `202` | `NOT_AUTHENTICATED` | commande envoyée avant `CONNECT`. Placé dans la famille 2xx, celle de `201 NAME_IN_USE`, qui est la famille d'authentification de la RFC |
| `400` | `BAD_REQUEST` | commande inconnue, argument manquant ou scope de chat invalide |
| `404` | `ROOM_NOT_FOUND` | la salle du joueur est introuvable dans le monde. La RFC ne définit 404 que pour ITEM et NPC ; c'est la même famille « not found » |
| `403` | `ACCESS_DENIED` | salle verrouillée (`ai_core`, tant que personne n'y est entré avec le `red_keycard`) |
| `407` | `ITEM_NOT_OBTAINABLE` | objet présent mais `obtainable: false` (décor fixe) |
| `408` | `INVENTORY_FULL` | inventaire plein (5 emplacements) |
| `409` | `NPC_ALREADY_DEAD` | cible déjà vaincue |
| `410` | `PLAYER_NOT_FOUND` | joueur cible d'un `GROUP INVITE` / `GROUP JOIN` inconnu |
| `429` | `TOO_MANY_REQUESTS` | anti-flood (plus de 5 commandes par seconde) |

Événement ajouté, absent de la RFC :

| Événement | Sens |
|---|---|
| `EVT PLAYER DEATH respawn=<room-id>` | le joueur est mort et réapparaît dans la salle indiquée |
| `EVT ROOM COMBAT <joueur> damage=<d> target_hp=<hp>` | résultat d'un round de combat, diffusé aux autres joueurs de la salle |
| `EVT ROOM COMBAT <joueur> defeated` | un joueur vient d'être vaincu dans la salle |

Justification : la RFC ne prévoit ni événement de mort ni événement de combat, alors que
le sujet impose un respawn et la diffusion des résultats de combat aux joueurs concernés.
Les deux suivent la grammaire `EVT <category> <type> <data>` de la, et `ROOM COMBAT`
reste dans la catégorie `ROOM` déjà définie par la RFC.

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
| `server_world.c` | `world_state(mode, key)` | état de monde partagé. Fonction à état statique avec un paramètre `mode`, sur le modèle de `manage_taken()` et `manage_dropped()` |
| `server_quests.c` | `reward_override()`, `append_quest()` | récompense du kill de l'`alpha_drone` et construction d'une entrée de `QUESTS` ; sortent `handle_attack` et `handle_quests` de la limite de 25 lignes |
| `server_cmd_move.c` | `ai_core_locked()` | verrou de l'`ai_core`, sorti de `handle_move` pour la même raison |
| `server_logs.c` | `send_reply()`, `escape_json()` | journalisation de toutes les réponses, et échappement JSON des champs contrôlés par le joueur |
| `server_resolve.c` | `get_quest_reward()` | lecture de la récompense d'une quête dans `data/world.json` |
| `server_cmd_drop.c` | `append_room_item()` | filtrage d'un objet déjà ramassé lors de la construction de la liste d'objets d'une salle ; sort `build_items_list` de la limite de 25 lignes |

Les 55 appels `write(p->sd, …)` ont été remplacés par `send_reply(p, …)`. Les diffusions
vers les autres joueurs (`write(c[i].sd, …)`) restent inchangées : ce sont des événements,
pas des réponses, et ils sont déjà journalisés à leur source.

Correctifs `Makefile` :

- la règle `$(SERVER)` n'avait **aucune dépendance**, donc `make` considérait
  `server_app` à jour même après modification des sources — il fallait un `make re` pour
  reconstruire. Elle dépend maintenant de `$(SERVER_SRC)` et `$(SERVER_HEADERS)` ;
- `SERVER_SRC` référençait `Process_buffer.c`, un fichier inexistant qui faisait échouer
  `make lint` ;
- `broadcast_player_stats.c` était compilé par la recette mais **absent de
  `SERVER_SRC`** : le modifier ne déclenchait aucune reconstruction, et `make lint` ne
  le vérifiait jamais. Il y est ajouté, et la recette utilise désormais `$(SERVER_SRC)`
  au lieu de redonner la liste des fichiers — les deux ne peuvent plus diverger ;
- `$(CLIENT)` ne dépendait pas de `$(CLIENT_HEADERS)` : modifier `cli/include/client.h`
  ne reconstruisait rien.

Vérifié après correction : `make` deux fois de suite ne fait rien, et toucher n'importe
quel source ou header déclenche exactement la reconstruction attendue — y compris
`layout.h`, qui recompile les 24 objets du GUI.

Signature modifiée : `handle_drop(t_player *, char *, char *)` — le paramètre
`t_player *c` est devenu inutile, `DROP` ne renvoyant plus qu'une seule réponse.

## 5. Impact sur les clients

- **GUI** : compatible sans modification. `OK room=` déclenche déjà un `LOOK`, les JSON
  de `DEFEND`/`FLEE` passent par `rfc_apply_combat`, et `EVT PLAYER DEATH` s'affiche
  dans le panneau de logs, les HP étant remis à jour par le `STATUS` qui suit.
- **Conséquence** : le parseur historique du GUI (`COMBAT:`, `EVENT:`, `DEAD:`, `NPC:`,
  `DROPPED:`) ne recevait plus rien du serveur. Il a été supprimé, cf.
  `gui/CORRECTIONS.md`. Une ligne inconnue est désormais simplement affichée dans le
  panneau de logs, ce qui reste un filet de sécurité face au serveur d'un autre groupe.
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
| Quête déjà résolue par quelqu'un | `QUEST <pnj>` renvoie `ERR 406 NO_QUEST_AVAILABLE` aux joueurs suivants, conformément à la RFC : *« NPC has no quests or quest already completed »* |
| Dialogue après résolution | L'ingénieur répond « System restored. Thanks! » et le terminal « Lockdown lifted. » à **tout le monde**, plus seulement à celui qui a agi |
| `QUESTS` d'un joueur ayant pris la quête avant sa résolution | l'entrée passe à `completed`, au lieu de rester bloquée en `active 0/1` sur un objectif devenu impossible |
| Porte de l'`ai_core` | le `red_keycard` est exigé pour la **première** ouverture ; ensuite la salle reste ouverte à tous. Cohérent avec le dialogue du `core_mainframe` : *« Lockdown lifted. You are authorized to access the escape shuttles. »* Le confinement est un état de la station, pas un privilège personnel |
| Mort d'un joueur | l'inventaire **tombe au sol** dans la salle où il est mort, au lieu d'être détruit. Récupérable par lui-même ou par un autre joueur |
| `DEFEND` | lève une garde : ne coûte rien, n'inflige rien, et divise par deux la riposte de la **prochaine** attaque du joueur. La garde retombe après cette attaque, à la mort et au changement de salle |

`DEFEND` ne servait auparavant à rien : il retirait 5-10 PV au joueur, n'infligeait aucun
dégât et ne laissait aucun effet — il était donc **strictement moins bon que ne rien
faire**, puisque les PNJ ne frappent qu'en riposte à un `ATTACK`. Le champ `defending`
de `t_player` lui donne la sémantique classique d'une garde, celle que le README décrit
déjà. Mesuré sur trois parties : 19, 20 et 12 PV perdus sans garde, contre 7, 9 et 7 avec.
La boucle de combat optimale devient `DEFEND` → `ATTACK`, soit deux fois plus de commandes
pour deux fois moins de dégâts subis.

Sans la dernière règle, un joueur mourant en portant le `red_keycard` détruisait
définitivement le seul exemplaire existant — le drone étant mort, aucun autre ne pouvait
être obtenu, et l'`ai_core` devenait inaccessible à tous jusqu'au redémarrage du serveur.
Le partage d'objets entre joueurs passe donc par le `DROP` ou par la mort, jamais par la
duplication.

Les trois drapeaux (`repair_done`, `override_done`, `ai_core_open`) vivent dans
`world_state()` et ne sont pas persistés : redémarrer le serveur réinitialise le monde,
ce que le sujet autorise explicitement (*« No persistence is required; state may reset
when the server restarts »*).
