/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_defend.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 22:06:12 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/17 22:09:13 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_defend(t_player *player, t_player *c, char *tgt, char *world)
{
	int		n_dmg;
	int		t_hp;
	char	id[32];
	char	msg[192];

	id[0] = '\0';
	if (tgt[0] != '\0')
		resolve_id(world, "\"npcs\": {", tgt, id);
	if (id[0] == '\0' || check_npc_in_room(world, player->location, id) == 0)
		return ((void)write(player->sd, "ERR 404 NPC_NOT_FOUND\n", 22));
	n_dmg = (10 + (rand() % 11)) / 2;
	player->hp -= n_dmg;
	player->in_combat = 1;
	server_log("INFO", player->ip, player->name, "Defend round resolved");
	if (player->hp <= 0)
		return (handle_death(player, c, world));
	t_hp = apply_damage_to_npc(player->location, id, 0, world);
	sprintf(msg, "OK {\"attacker_hp\": %d, \"target_hp\": %d, "
		"\"damage\": %d, \"status\": \"defend\"}\n",
		player->hp, t_hp, n_dmg);
	write(player->sd, msg, strlen(msg));
}

static void	flee_success(t_player *player, t_player *c)
{
	char	msg[128];

	sprintf(msg, "EVT ROOM PRESENCE LEAVE %s\n", player->name);
	broadcast_room(c, player->location, msg, player->sd);
	strncpy(player->location, player->last_location, 31);
	player->location[31] = '\0';
	player->in_combat = 0;
	sprintf(msg, "EVT ROOM PRESENCE ENTER %s\n", player->name);
	broadcast_room(c, player->location, msg, player->sd);
	server_log("INFO", player->ip, player->name, "Fled from combat");
	sprintf(msg, "OK room=%s\n", player->location);
	write(player->sd, msg, strlen(msg));
}

void	handle_flee(t_player *player, t_player *c, char *world)
{
	int		n_dmg;
	char	msg[192];

	if (rand() % 2 != 0)
		return (flee_success(player, c));
	n_dmg = 10 + (rand() % 11);
	player->hp -= n_dmg;
	player->in_combat = 1;
	server_log("INFO", player->ip, player->name, "Flee attempt failed");
	if (player->hp <= 0)
		return (handle_death(player, c, world));
	sprintf(msg, "OK {\"attacker_hp\": %d, \"target_hp\": 0, "
		"\"damage\": %d, \"status\": \"flee_failed\"}\n",
		player->hp, n_dmg);
	write(player->sd, msg, strlen(msg));
}
