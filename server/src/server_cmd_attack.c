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
	int		i;
	char	evt[128];

	sprintf(evt, "EVT ROOM COMBAT %s defeated\n", player->name);
	broadcast_room(c, player->location, evt, player->sd);
	i = 0;
	while (i < 5)
	{
		if (player->inventory[i][0] != '\0')
			manage_dropped(1, player->location, player->inventory[i], NULL);
		player->inventory[i][0] = '\0';
		i++;
	}
	strncpy(player->location, "medbay", 31);
	player->location[31] = '\0';
	player->hp = 50;
	player->in_combat = 0;
	player->defending = 0;
	server_log("INFO", player->ip, player->name, "Player died and respawned");
	send_reply(player, "EVT PLAYER DEATH respawn=medbay\n");
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

static void	send_combat_msg(t_player *p, t_player *c, int d, int t_hp)
{
	char	msg[256];

	if (t_hp <= 0)
		sprintf(msg, "OK {\"attacker_hp\": %d, \"target_hp\": 0, "
			"\"damage\": %d, \"status\": \"victory\"}\n", p->hp, d);
	else
		sprintf(msg, "OK {\"attacker_hp\": %d, \"target_hp\": %d, "
			"\"damage\": %d, \"status\": \"combat\"}\n", p->hp, t_hp, d);
	server_log("INFO", p->ip, p->name, "Combat round resolved");
	send_reply(p, msg);
	sprintf(msg, "EVT ROOM COMBAT %s damage=%d target_hp=%d\n",
		p->name, d, t_hp * (t_hp > 0));
	broadcast_room(c, p->location, msg, p->sd);
}

static int	check_attack_err(t_player *p, char *id, char *wld)
{
	if (id[0] == '\0' || !check_npc_in_room(wld, p->location, id))
	{
		send_reply(p, "ERR 404 NPC_NOT_FOUND\n");
		return (1);
	}
	if (!is_npc_enemy(wld, id))
	{
		send_reply(p, "ERR 405 NPC_NOT_HOSTILE\n");
		return (1);
	}
	if (apply_damage_to_npc(p->location, id, 0, wld) <= 0)
	{
		send_reply(p, "ERR 409 NPC_ALREADY_DEAD\n");
		return (1);
	}
	return (0);
}

void	handle_attack(t_player *p, t_player *c, char *tgt, char *wld)
{
	int		d;
	int		cd;
	int		t_hp;
	char	id[32];

	id[0] = '\0';
	if (tgt[0] != '\0')
		resolve_id(wld, "\"npcs\": {", tgt, id);
	if (check_attack_err(p, id, wld) == 1)
		return ;
	d = 15 + (rand() % 11);
	if (has_item(p, "laser_pistol"))
		d = 30 + (rand() % 11);
	t_hp = apply_damage_to_npc(p->location, id, d, wld);
	p->in_combat = (t_hp > 0);
	if (t_hp <= 0 && !strcmp(id, "alpha_drone"))
		reward_override(p);
	cd = (10 + (rand() % 11)) / (1 + p->defending);
	p->defending = 0;
	if (t_hp > 0)
		p->hp -= cd;
	if (p->hp <= 0)
		return (handle_death(p, c, wld));
	send_combat_msg(p, c, d, t_hp);
}
