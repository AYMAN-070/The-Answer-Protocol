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

static void	send_quest(t_player *p, char *id, char *desc, int taken)
{
	char	o[256];
	char	*st;
	char	*rw;

	st = "available";
	if (taken == 1)
		st = "active";
	rw = "laser_pistol";
	if (!strcmp(id, "override"))
		rw = "red_keycard";
	sprintf(o, "OK {\"quest_id\": \"%s\", \"description\": \"%s\", "
		"\"reward\": \"%s\", \"status\": \"%s\"}\n", id, desc, rw, st);
	write(p->sd, o, strlen(o));
}

void	handle_quest(t_player *p, char *npc, char *wld)
{
	char	id[32];

	id[0] = '\0';
	if (npc && npc[0] != '\0')
		resolve_id(wld, "\"npcs\": {", npc, id);
	if (id[0] == '\0' || !check_npc_in_room(wld, p->location, id))
		return ((void)write(p->sd, "ERR 404 NPC_NOT_FOUND\n", 22));
	if (!strcmp(id, "injured_engineer") && p->quest_status < 2)
	{
		send_quest(p, "repair", "Bring an energy cell", p->quest_status);
		p->quest_status = 1;
		return (server_log("INFO", p->ip, p->name, "Quest repair offered"));
	}
	if (!strcmp(id, "ai_terminal") && p->quest_override < 2)
	{
		send_quest(p, "override", "Destroy the alpha drone", p->quest_override);
		p->quest_override = 1;
		return (server_log("INFO", p->ip, p->name, "Quest override offered"));
	}
	write(p->sd, "ERR 406 NO_QUEST_AVAILABLE\n", 27);
}

void	handle_quests(t_player *p)
{
	char	o[256];
	char	*st;
	char	*pr;

	strcpy(o, "OK [");
	if (p->quest_status > 0)
	{
		st = p->quest_status > 1 ? "completed" : "active";
		pr = p->quest_status > 1 ? "1/1" : "0/1";
		sprintf(o + strlen(o), "{\"quest_id\": \"repair\", "
			"\"status\": \"%s\", \"progress\": \"%s\"}", st, pr);
	}
	if (p->quest_status > 0 && p->quest_override > 0)
		strcat(o, ", ");
	if (p->quest_override > 0)
	{
		st = p->quest_override > 1 ? "completed" : "active";
		pr = p->quest_override > 1 ? "1/1" : "0/1";
		sprintf(o + strlen(o), "{\"quest_id\": \"override\", "
			"\"status\": \"%s\", \"progress\": \"%s\"}", st, pr);
	}
	strcat(o, "]\n");
	write(p->sd, o, strlen(o));
}
