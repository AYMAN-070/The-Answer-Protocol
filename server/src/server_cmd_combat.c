/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_combat.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 14:53:02 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/09 23:04:02 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <stdlib.h>

void	handle_death(t_player *player)
{
	int	i;

	i = 0;
	while (i < 5)
	{
		player->inventory[i][0] = '\0';
		i++;
	}
	strncpy(player->location, "medbay", 31);
	player->location[31] = '\0';
	player->hp = 50;
	write(player->sd, "DEAD:You died! Respawned in medbay.\n", 36);
}

void	give_item(t_player *player, char *item)
{
	int	i;

	i = 0;
	while (i < 5)
	{
		if (player->inventory[i][0] == '\0')
		{
			strncpy(player->inventory[i], item, 31);
			player->inventory[i][31] = '\0';
			return ;
		}
		i++;
	}
	strncpy(player->inventory[4], item, 31);
	player->inventory[4][31] = '\0';
}

void	handle_attack(t_player *p, int w, char *tgt, char *wld)
{
	static int	d_hp = 50;
	int			dmg[2];
	char		msg[128];

	if (w != 2 || check_npc_in_room(wld, p->location, tgt) == 0)
		return ((void)write(p->sd, "ERR missing target\n", 19));
	dmg[0] = 15 + (rand() % 11);
	if (has_item(p, "laser_pistol") == 1)
		dmg[0] = 30 + (rand() % 11);
	if (strcmp(tgt, "alpha_drone") == 0 && d_hp > 0)
		d_hp -= dmg[0];
	if (strcmp(tgt, "alpha_drone") == 0 && d_hp <= 0)
	{
		write(p->sd, "EVENT:Drone dead! Got keycard\n", 30);
		give_item(p, "red_keycard");
		return ;
	}
	dmg[1] = 10 + (rand() % 11);
	p->hp -= dmg[1];
	if (p->hp <= 0)
		return (handle_death(p));
	sprintf(msg, "COMBAT:Hit %d. Npc hit %d. HP:%d\n", dmg[0], dmg[1], p->hp);
	write(p->sd, msg, strlen(msg));
}

void	handle_defend(t_player *player, int words, char *tgt, char *world)
{
	int		n_dmg;
	char	msg[128];

	if (words != 2 || check_npc_in_room(world, player->location, tgt) == 0)
		return ((void)write(player->sd, "ERR missing target\n", 19));
	n_dmg = (10 + (rand() % 11)) / 2;
	player->hp -= n_dmg;
	if (player->hp <= 0)
		return (handle_death(player));
	sprintf(msg, "COMBAT:You defend! Target hit %d. HP:%d\n",
		n_dmg, player->hp);
	write(player->sd, msg, strlen(msg));
}

void	handle_flee(t_player *player, char *world)
{
	int		n_dmg;
	char	msg[128];

	if (rand() % 2 == 0)
	{
		n_dmg = 10 + (rand() % 11);
		player->hp -= n_dmg;
		if (player->hp <= 0)
			return (handle_death(player));
		sprintf(msg, "COMBAT:Flee failed! Target hit %d. HP:%d\n",
			n_dmg, player->hp);
		write(player->sd, msg, strlen(msg));
	}
	else
	{
		strncpy(player->location, player->last_location, 31);
		write(player->sd, "EVENT:Flee successful!\n", 23);
		handle_look(player, world);
	}
}
