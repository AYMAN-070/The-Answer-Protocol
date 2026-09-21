/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_quests.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 20:35:45 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/19 13:35:02 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static void	send_quest(t_player *p, char *id, char *wld, int taken)
{
	char	o[256];
	char	rw[32];
	char	*st;
	char	*desc;

	st = "available";
	if (taken == 1)
		st = "active";
	desc = "Bring an energy cell to the injured engineer";
	if (!strcmp(id, "system_override"))
		desc = "Destroy the alpha drone";
	get_quest_reward(wld, id, rw);
	sprintf(o, "OK {\"quest_id\": \"%s\", \"description\": \"%s\", "
		"\"reward\": \"%s\", \"status\": \"%s\"}\n", id, desc, rw, st);
	send_reply(p, o);
}

void	handle_quest(t_player *p, char *npc, char *wld)
{
	char	id[32];

	id[0] = '\0';
	if (npc && npc[0] != '\0')
		resolve_id(wld, "\"npcs\": {", npc, id);
	if (id[0] == '\0' || !check_npc_in_room(wld, p->location, id))
		return ((void)send_reply(p, "ERR 404 NPC_NOT_FOUND\n"));
	if (!strcmp(id, "injured_engineer") && p->quest_status < 2
		&& !world_state(1, "repair_done"))
	{
		send_quest(p, "power_up", wld, p->quest_status);
		p->quest_status = 1;
		return (server_log("INFO", p->ip, p->name, "Quest repair offered"));
	}
	if (!strcmp(id, "ai_terminal") && p->quest_override < 2
		&& !world_state(1, "override_done"))
	{
		send_quest(p, "system_override", wld, p->quest_override);
		p->quest_override = 1;
		return (server_log("INFO", p->ip, p->name, "Quest override offered"));
	}
	send_reply(p, "ERR 406 NO_QUEST_AVAILABLE\n");
}

void	reward_override(t_player *p)
{
	give_item(p, "red_keycard");
	p->quest_override = 2;
	world_state(2, "override_done");
	server_log("INFO", p->ip, p->name, "Quest override completed");
}

static void	append_quest(char *o, char *id, int done)
{
	char	*st;
	char	*pr;

	st = "active";
	pr = "0/1";
	if (done)
	{
		st = "completed";
		pr = "1/1";
	}
	sprintf(o + strlen(o), "{\"quest_id\": \"%s\", "
		"\"status\": \"%s\", \"progress\": \"%s\"}", id, st, pr);
}

void	handle_quests(t_player *p)
{
	char	o[256];
	int		rdone;
	int		odone;

	rdone = (p->quest_status > 1 || world_state(1, "repair_done"));
	odone = (p->quest_override > 1 || world_state(1, "override_done"));
	strcpy(o, "OK [");
	if (p->quest_status > 0)
		append_quest(o, "power_up", rdone);
	if (p->quest_status > 0 && p->quest_override > 0)
		strcat(o, ", ");
	if (p->quest_override > 0)
		append_quest(o, "system_override", odone);
	strcat(o, "]\n");
	send_reply(p, o);
}
