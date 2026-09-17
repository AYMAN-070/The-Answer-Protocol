/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_talk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 17:23:52 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/17 15:01:23 by ayhammou         ###   ########.fr       */
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

void	handle_talk(t_player *player, int words, char *npc, char *world)
{
	int	i;

	if (words != 2 || check_npc_in_room(world, player->location, npc) == 0)
	{
		write(player->sd, "ERR 404 NPC_NOT_FOUND\n", 22);
		return ;
	}
	if (strcmp(npc, "injured_engineer") == 0)
	{
		if (has_item(player, "energy_cell") == 1)
		{
			player->quest_status = 2;
			i = -1;
			while (++i < 5)
				if (strcmp(player->inventory[i], "energy_cell") == 0)
					player->inventory[i][0] = '\0';
			write(player->sd, "OK Quest completed! Got laser_pistol\n", 37);
			give_item(player, "laser_pistol");
		}
		else
			write(player->sd, "OK Please, find an energy_cell...\n", 35);
	}
	else
		write(player->sd, "OK I have nothing to say to you.\n", 34);
}
