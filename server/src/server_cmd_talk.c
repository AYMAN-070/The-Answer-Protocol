/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_talk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 17:23:52 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/17 22:12:46 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_status(t_player *p)
{
	char	msg[128];

	sprintf(msg, "OK {\"hp\": %d, \"max_hp\": %d, \"status\": \"healthy\"}\n",
		p->hp, p->max_hp);
	write(p->sd, msg, strlen(msg));
}

void	handle_quit(t_player *player)
{
	server_log("INFO", player->ip, player->name, "Player quit");
	write(player->sd, "OK bye\n", 7);
	close(player->sd);
	player->is_active = 0;
	player->sd = -1;
	memset(player->name, 0, 32);
	memset(player->inventory, 0, sizeof(player->inventory));
	server_log("INFO", player->ip,
		"UNKNOWN", "Player disconnected and slot freed");
}

void	handle_talk(t_player *p, int words, char *npc, char *wld)
{
	char	d[256];
	char	o[256];
	int		i;

	if (words != 2 || !check_npc_in_room(wld, p->location, npc))
		return ((void)write(p->sd, "ERR 404 NPC_NOT_FOUND\n", 22));
	if (strcmp(npc, "injured_engineer") == 0 && p->quest_status == 2)
		return ((void)write(p->sd, "OK System restored. Thanks!\n", 28));
	if (strcmp(npc, "injured_engineer") == 0 && has_item(p, "energy_cell"))
	{
		p->quest_status = 2;
		i = -1;
		while (++i < 5)
			if (strcmp(p->inventory[i], "energy_cell") == 0)
				p->inventory[i][0] = '\0';
		give_item(p, "laser_pistol");
		return ((void)write(p->sd, "OK Quest completed!\n", 20));
	}
	get_npc_dialogue(wld, npc, d);
	snprintf(o, sizeof(o), "OK %.245s\n", d);
	write(p->sd, o, strlen(o));
}

