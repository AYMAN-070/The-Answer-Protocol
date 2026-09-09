/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 19:07:59 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/09 22:27:59 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

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

void	handle_drop(t_player *player, int words, char *item)
{
	int		i;
	char	msg[128];

	if (words != 2)
		return ((void)write(player->sd, "ERR missing item\n", 17));
	i = -1;
	while (++i < 5)
	{
		if (strcmp(player->inventory[i], item) == 0)
		{
			player->inventory[i][0] = '\0';
			sprintf(msg, "OK dropped %s\n", item);
			write(player->sd, msg, strlen(msg));
			return ;
		}
	}
	write(player->sd, "ERR item not found\n", 19);
}
