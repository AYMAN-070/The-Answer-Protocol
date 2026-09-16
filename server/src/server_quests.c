/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_quests.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 20:35:45 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/16 21:08:32 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_quest(t_player *p, char *npc_name, char *wld)
{
	if (npc_name == NULL || npc_name[0] == '\0')
		return ((void)write(p->sd, "ERR missing argument\n", 21));
	if (check_npc_in_room(wld, p->location, npc_name) == 0)
		return ((void)write(p->sd, "ERR 404 NPC_NOT_FOUND\n", 22));
	if (strcmp(npc_name, "injured_engineer") != 0)
		return ((void)write(p->sd, "ERR 406 NO_QUEST_AVAILABLE\n", 27));
	if (p->quest_status != 0)
		return ((void)write(p->sd, "ERR 406 NO_QUEST_AVAILABLE\n", 27));
	p->quest_status = 1;
	write(p->sd, "OK {\"quest_id\": \"repair\", \"reward\": \"key\"}\n", 44);
}

void	handle_quests(t_player *p)
{
	if (p->quest_status == 0)
		write(p->sd, "OK []\n", 6);
	else if (p->quest_status == 1)
		write(p->sd, "OK [{\"quest_id\": \"repair\", "
			"\"status\": \"active\"}]\n", 49);
	else if (p->quest_status == 2)
		write(p->sd, "OK [{\"quest_id\": \"repair\", "
			"\"status\": \"completed\"}]\n", 52);
}
