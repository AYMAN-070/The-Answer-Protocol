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
	char	msg[128];

	if (tgt[0] == '\0' || check_npc_in_room(world, player->location, tgt) == 0)
		return ((void)write(player->sd, "ERR missing target\n", 19));
	n_dmg = (10 + (rand() % 11)) / 2;
	player->hp -= n_dmg;
	if (player->hp <= 0)
	{
		handle_death(player, c, world);
		return ;
	}
	sprintf(msg, "COMBAT:You defend! Target hit %d. HP:%d\n",
		n_dmg, player->hp);
	write(player->sd, msg, strlen(msg));
}

void	handle_flee(t_player *player, t_player *c, char *world)
{
	int		n_dmg;
	char	msg[128];

	if (rand() % 2 == 0)
	{
		n_dmg = 10 + (rand() % 11);
		player->hp -= n_dmg;
		if (player->hp <= 0)
		{
			handle_death(player, c, world);
			return ;
		}
		sprintf(msg, "COMBAT:Flee failed! Target hit %d. HP:%d\n",
			n_dmg, player->hp);
		write(player->sd, msg, strlen(msg));
	}
	else
	{
		strncpy(player->location, player->last_location, 31);
		write(player->sd, "EVENT:Flee successful!\n", 23);
		handle_look(player, c, world);
	}
}
