## 1. Conformité RFC 42TAP

| Point | Avant | Après |
|---|---|---|
| `QUIT` | intercepté **localement** (`!strncmp(buffer, "QUIT\n", 5)` → `return (0)`), la socket était fermée sans que la commande parte. Le serveur ne recevait jamais `QUIT`, ne répondait jamais `OK bye`, et la déconnexion passait systématiquement par le chemin brutal (`read() <= 0`) | la ligne est envoyée au serveur comme n'importe quelle autre commande. Le serveur répond `OK bye` puis ferme ; la boucle se termine sur le `read()` à 0 |
| Réassemblage des lignes | `read()` puis `printf` du buffer brut. Deux réponses arrivées dans le même paquet TCP s'affichaient collées, et une réponse coupée en deux paquets s'affichait en deux morceaux | accumulation dans un buffer persistant, découpage sur `\n`, une ligne affichée à la fois, le reste conservé pour le prochain `read()` |

Le découpage reprend exactement le motif déjà utilisé côté serveur dans
`read_client_data()` (`strchr('\n')` + `memmove` du reliquat), pour rester cohérent
avec le code existant.

## 2. Conformité sujet (TAP v1.4)

| Exigence | Avant | Après |
|---|---|---|
| « CLI and GUI clients must be interchangeable between different groups » | IP et port écrits en dur (`127.0.0.1`, `4242`), aucun argument : impossible de se connecter au serveur d'un autre groupe | `./client_app [ip] [port]`, avec les mêmes valeurs par défaut. `make run-client` fonctionne sans argument |

## 3. Fichiers ajoutés

| Fichier | Contenu | Raison |
|---|---|---|
| `cli/include/client.h` (nouveau) | includes communs + prototypes | le `Makefile` passait déjà `-I cli/include` alors que le dossier n'existait pas. Même structure que `server/include/server.h` |
| `cli/src/client_utils.c` (nouveau) | `exit_error()`, `print_lines()` | `client.c` était déjà à 5 fonctions, soit la limite de la norme. Le découpage des lignes ne pouvait pas y être ajouté |

Modifications du `Makefile` :

- `CLIENT_SRC` inclut `cli/src/client_utils.c` (la règle `$(CLIENT)` utilise déjà
  `$(CLIENT_SRC)`, elle n'a pas eu à changer) ;
- ajout de `CLIENT_HEADERS = cli/include/client.h`, repris dans `LINT_HDR` pour que
  `make lint` vérifie aussi le header du CLI.

Fonction ajoutée dans `client.c` : `connect_to_server(argc, argv)`, qui regroupe la
lecture des arguments, la création de la socket et la connexion. `main()` se limite
désormais à l'enchaînement connexion → boucle → fermeture.

Signature modifiée : `handle_server_msg(int sock, char *buffer, int *len)` — la longueur
courante du buffer de réception doit persister entre deux appels pour permettre le
réassemblage.

## 4. Impact

- **Affichage** : chaque ligne du serveur est précédée de `Serveur : ` et suivie d'un
  retour à la ligne, au lieu d'un bloc brut. Le `\r` d'origine est conservé, il efface
  l'invite `> ` dans un vrai terminal.
- **Sortie** : taper `QUIT` produit maintenant `Serveur : OK bye` avant la fermeture,
  et le processus sort avec le code 0.
- **Serveur** : aucune modification nécessaire, le CLI n'utilise que des commandes déjà
  implémentées.
