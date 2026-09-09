/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_talk.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 17:23:52 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/09 22:01:49 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_talk(t_player *player, int words, char *npc, char *world)
{
	int	i;

	if (words != 2 || check_npc_in_room(world, player->location, npc) == 0)
		return ((void)write(player->sd, "ERR missing or invalid npc\n", 27));
	if (strcmp(npc, "injured_engineer") == 0)
	{
		if (has_item(player, "energy_cell") == 1)
		{
			i = -1;
			while (++i < 5)
				if (strcmp(player->inventory[i], "energy_cell") == 0)
					player->inventory[i][0] = '\0';
			write(player->sd, "EVENT:Quest completed! Got laser_pistol\n", 40);
			give_item(player, "laser_pistol");
		}
		else
			write(player->sd, "NPC:Please, find an energy_cell...\n", 35);
	}
	else
		write(player->sd, "NPC:I have nothing to say to you.\n", 34);
}
