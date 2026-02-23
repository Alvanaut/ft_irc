# ft_irc -- Répartition à 3

## Personne A : Coeur du serveur (Networking)
> C'est la fondation sur laquelle tout le reste repose.

- [ ] **Makefile** (NAME, all, clean, fclean, re, flags `-Wall -Wextra -Werror -std=c++98`)
- [ ] **main / parsing des arguments** (`./ircserv <port> <password>`)
- [ ] **Classe Server** : socket, bind, listen, poll() loop
- [ ] **Gestion des connexions** : accept, non-blocking I/O (`fcntl(fd, F_SETFL, O_NONBLOCK)`)
- [ ] **Buffer de reception** : aggreger les paquets partiels (test `nc` avec ctrl+D)
- [ ] **Classe Client** : fd, buffer, nickname, username, état d'authentification
- [ ] **Gestion des signaux** (SIGINT, SIGPIPE, etc.)
- [ ] **Deconnexion propre** d'un client (QUIT, fermeture socket)

---

## Personne B : Authentification + Commandes utilisateur
> Les commandes de base que chaque client utilise.

- [ ] **PASS** : vérification du mot de passe serveur
- [ ] **NICK** : set/change nickname (collisions, validation)
- [ ] **USER** : set username/realname
- [ ] **Flux d'enregistrement** : PASS -> NICK -> USER (welcome replies RPL_WELCOME etc.)
- [ ] **PRIVMSG** : messages privés (user-to-user)
- [ ] **PRIVMSG** : messages vers un channel (forward à tous les membres)
- [ ] **NOTICE**
- [ ] **PING / PONG**
- [ ] **Numeric replies / error codes** (ERR_NEEDMOREPARAMS, ERR_ALREADYREGISTRED, etc.)
- [ ] **Command parser** : découper les messages IRC (`:<prefix> <command> <params>`)

---

## Personne C : Channels + Commandes opérateur
> Tout ce qui concerne les channels et les droits opérateur.

- [ ] **Classe Channel** : nom, topic, liste de membres, liste d'opérateurs, modes
- [ ] **JOIN** : rejoindre un channel (créer si inexistant, devenir op du premier)
- [ ] **PART** : quitter un channel
- [ ] **KICK** : éjecter un client (op only)
- [ ] **INVITE** : inviter un client (op only)
- [ ] **TOPIC** : voir/changer le topic
- [ ] **MODE** avec tous les flags :
  - `i` : invite-only
  - `t` : restriction TOPIC aux ops
  - `k` : channel key (password)
  - `o` : give/take operator
  - `l` : user limit
- [ ] **NAMES / WHO** (liste des users dans un channel)
- [ ] **README.md**

---

## Taches communes (les 3 ensemble)
- [ ] Choisir le **client IRC de référence** (irssi, HexChat, LimeChat, WeeChat...)
- [ ] **Tests** avec `nc` et le client de référence
- [ ] **Debugging** ensemble avant la défense
- [ ] **Bonus** (bot, file transfer) -- seulement si la mandatory est parfaite

---

## Ordre de travail suggéré
1. **A** commence le serveur (socket + poll loop + accept) -- les autres ne peuvent pas tester sans ça
2. **B** enchaîne avec le parser + PASS/NICK/USER dès que A peut accepter des connexions
3. **C** démarre la classe Channel en parallèle et branche JOIN/PART quand B a le parser prêt

> A débloque B et C, et B fournit le parser que C utilise aussi.
> Chacun bosse sur sa branche git et vous mergez au fur et à mesure.
