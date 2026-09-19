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

void	handle_quest(t_player *p, char *npc, char *wld)
{
	char	*o;

	if (!npc || npc[0] == '\0' || !check_npc_in_room(wld, p->location, npc))
		return ((void)write(p->sd, "ERR 404 NPC_NOT_FOUND\n", 22));
	if (!strcmp(npc, "injured_engineer") && p->quest_status < 2)
	{
		p->quest_status = 1;
		o = "OK {\"quest_id\": \"repair\", \"description\": \"Fix core\", "
			"\"reward\": \"key\", \"status\": \"available\"}\n";
		return ((void)write(p->sd, o, strlen(o)));
	}
	if (!strcmp(npc, "ai_terminal") && p->quest_override < 2)
	{
		p->quest_override = 1;
		o = "OK {\"quest_id\": \"override\", \"description\": \"Stop AI\", "
			"\"reward\": \"keycard\", \"status\": \"available\"}\n";
		return ((void)write(p->sd, o, strlen(o)));
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
