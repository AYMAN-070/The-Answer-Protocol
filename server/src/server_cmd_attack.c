/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_attack.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 14:53:02 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/19 14:00:59 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <stdlib.h>

void	handle_death(t_player *player, t_player *c, char *world)
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
	handle_look(player, c, world);
	handle_inventory(player);
	handle_status(player);
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

static void	send_combat_msg(t_player *p, int d, int t_hp)
{
	char	msg[256];

	if (t_hp <= 0)
		sprintf(msg, "OK {\"attacker_hp\": %d, \"target_hp\": 0, "
			"\"damage\": %d, \"status\": \"victory\"}\n", p->hp, d);
	else
		sprintf(msg, "OK {\"attacker_hp\": %d, \"target_hp\": %d, "
			"\"damage\": %d, \"status\": \"combat\"}\n", p->hp, t_hp, d);
	write(p->sd, msg, strlen(msg));
}

void	handle_attack(t_player *p, t_player *c, char *tgt, char *wld)
{
	int	d;
	int	t_hp;

	if (tgt[0] == '\0' || !check_npc_in_room(wld, p->location, tgt))
		return ((void)write(p->sd, "ERR 404 NPC_NOT_FOUND\n", 22));
	if (!is_npc_enemy(wld, tgt))
		return ((void)write(p->sd, "ERR 405 NPC_NOT_HOSTILE\n", 24));
	if (apply_damage_to_npc(p->location, tgt, 0, wld) <= 0)
		return ((void)write(p->sd, "ERR 406 NPC_ALREADY_DEAD\n", 25));
	d = 15 + (rand() % 11);
	if (has_item(p, "laser_pistol"))
		d = 30 + (rand() % 11);
	t_hp = apply_damage_to_npc(p->location, tgt, d, wld);
	if (t_hp <= 0 && !strcmp(tgt, "alpha_drone"))
	{
		give_item(p, "red_keycard");
		p->quest_override = 2;
	}
	if (t_hp > 0)
		p->hp -= (10 + (rand() % 11));
	if (p->hp <= 0)
		return (handle_death(p, c, wld));
	send_combat_msg(p, d, t_hp);
}

