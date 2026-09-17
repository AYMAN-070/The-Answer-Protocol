/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_quests.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 20:35:45 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/17 17:21:14 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_quest(t_player *p, char *npc, char *wld)
{
	char	*out;

	if (npc == NULL || npc[0] == '\0'
		|| !check_npc_in_room(wld, p->location, npc))
		return ((void)write(p->sd, "ERR 404 NPC_NOT_FOUND\n", 22));
	if (strcmp(npc, "injured_engineer") != 0 || p->quest_status != 0)
		return ((void)write(p->sd, "ERR 406 NO_QUEST_AVAILABLE\n", 27));
	p->quest_status = 1;
	out = "OK {\"quest_id\": \"repair\", \"description\": \"Fix core\", "
		"\"reward\": \"key\", \"status\": \"available\"}\n";
	write(p->sd, out, strlen(out));
}

void	handle_quests(t_player *p)
{
	if (p->quest_status == 0)
		write(p->sd, "OK []\n", 6);
	else if (p->quest_status == 1)
		write(p->sd, "OK [{\"quest_id\": \"repair\", "
			"\"status\": \"active\", \"progress\": \"0/1\"}]\n", 67);
	else if (p->quest_status == 2)
		write(p->sd, "OK [{\"quest_id\": \"repair\", "
			"\"status\": \"completed\", \"progress\": \"1/1\"}]\n", 70);
}