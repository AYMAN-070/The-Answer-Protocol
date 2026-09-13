/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 19:07:59 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/13 18:05:11 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_quit(t_player *player)
{
	server_log("INFO", player->ip, player->name, "Player quit");
	write(player->sd, "OK goodbye\n", 11);
}

void	handle_status(t_player *player)
{
	char	msg[128];

	sprintf(msg, "STATUS:HP:%d/100 ROOM:%s\n", player->hp, player->location);
	write(player->sd, msg, strlen(msg));
}

void	handle_quest(t_player *player)
{
	char	*quests;

	quests = "QUESTS:[1] Bring energy_cell to injured_engineer "
		"[2] Defeat alpha_drone\n";
	write(player->sd, quests, strlen(quests));
}

void	handle_drop(t_player *player, int words, char *item, char *world_data)
{
	int		i;
	char	msg[128];

	if (words != 2)
	{
		server_log("WARN", player->ip, player->name, "Drop missing item");
		return ((void)write(player->sd, "ERR missing item\n", 17));
	}
	i = -1;
	while (++i < 5)
	{
		if (strcmp(player->inventory[i], item) == 0)
		{
			player->inventory[i][0] = '\0';
			manage_dropped(1, player->location, item, NULL);
			server_log("INFO", player->ip, player->name, "Item dropped");
			sprintf(msg, "OK dropped %s\n", item);
			write(player->sd, msg, strlen(msg));
			handle_inventory(player);
			handle_look(player, world_data);
			return ;
		}
	}
	server_log("WARN", player->ip, player->name, "Drop item not found");
	write(player->sd, "ERR item not found\n", 19);
}
