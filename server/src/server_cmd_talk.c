/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_talk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 17:23:52 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/18 23:43:12 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_status(t_player *p)
{
	char	msg[128];
	char	*st;

	st = "healthy";
	if (p->hp < (p->max_hp / 2))
		st = "wounded";
	if (p->in_combat == 1)
		st = "in_combat";
	sprintf(msg, "OK {\"hp\": %d, \"max_hp\": %d, \"status\": \"%s\"}\n",
		p->hp, p->max_hp, st);
	write(p->sd, msg, strlen(msg));
}

void	handle_quit(t_player *player, t_player *c)
{
	char	out[128];

	server_log("INFO", player->ip, player->name, "Player quit");
	write(player->sd, "OK bye\n", 7);
	close(player->sd);
	player->is_active = 0;
	player->sd = -1;
	sprintf(out, "EVT ROOM PRESENCE LEAVE %s\n", player->name);
	broadcast_room(c, player->location, out, -1);
	broadcast_player_stats(c);
	memset(player->name, 0, 32);
	memset(player->inventory, 0, sizeof(player->inventory));
	server_log("INFO", player->ip,
		"UNKNOWN", "Player disconnected and slot freed");
}

static void	complete_repair_quest(t_player *p)
{
	int	i;

	p->quest_status = 2;
	i = -1;
	while (++i < 5)
		if (!strcmp(p->inventory[i], "energy_cell"))
			p->inventory[i][0] = '\0';
	give_item(p, "laser_pistol");
	server_log("INFO", p->ip, p->name, "Quest repair completed");
	write(p->sd, "OK Quest completed!\n", 20);
}

void	handle_disconnect(t_player *p, t_player *c)
{
	char	out[128];

	server_log("INFO", p->ip, p->name, "Disconnected");
	close(p->sd);
	p->is_active = 0;
	p->cmd_len = 0;
	memset(p->cmd_buffer, 0, sizeof(p->cmd_buffer));
	sprintf(out, "EVT ROOM PRESENCE LEAVE %s\n", p->name);
	broadcast_room(c, p->location, out, -1);
	broadcast_player_stats(c);
	memset(p->name, 0, 32);
}

void	handle_talk(t_player *p, int words, char *npc, char *wld)
{
	char	id[32];
	char	d[256];
	char	o[256];

	id[0] = '\0';
	if (words == 2)
		resolve_id(wld, "\"npcs\": {", npc, id);
	if (words != 2 || !check_npc_in_room(wld, p->location, id))
		return ((void)write(p->sd, "ERR 404 NPC_NOT_FOUND\n", 22));
	if (!strcmp(id, "injured_engineer") && p->quest_status == 2)
		return ((void)write(p->sd, "OK System restored. Thanks!\n", 28));
	if (!strcmp(id, "injured_engineer") && has_item(p, "energy_cell"))
		return (complete_repair_quest(p));
	if (!strcmp(id, "ai_terminal") && p->quest_override == 2)
		return ((void)write(p->sd, "OK Lockdown lifted.\n", 20));
	if (!strcmp(id, "ai_terminal") && p->quest_override == 0)
	{
		p->quest_override = 1;
		server_log("INFO", p->ip, p->name, "Quest override started");
	}
	get_npc_dialogue(wld, id, d);
	snprintf(o, sizeof(o), "OK %.245s\n", d);
	write(p->sd, o, strlen(o));
}
