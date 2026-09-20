/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_move.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 11:40:23 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/17 16:50:40 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	has_item(t_player *player, char *item)
{
	int	i;

	i = 0;
	while (i < 5)
	{
		if (strcmp(player->inventory[i], item) == 0)
			return (1);
		i++;
	}
	return (0);
}

static int	ai_core_locked(t_player *p, char *dest)
{
	if (strcmp(dest, "ai_core") != 0 || world_state(1, "ai_core_open"))
		return (0);
	if (has_item(p, "red_keycard") == 0)
	{
		write(p->sd, "ERR 403 ACCESS_DENIED\n", 22);
		return (1);
	}
	world_state(2, "ai_core_open");
	server_log("INFO", p->ip, p->name, "AI core unlocked for everyone");
	return (0);
}

void	handle_move(t_player *p, t_player *c, char *arg, char *wld)
{
	char	dest[32];
	char	out[128];

	dest[0] = '\0';
	if (arg[0] == '\0')
		return ((void)write(p->sd, "ERR 301 NO_EXIT\n", 16));
	get_room_exit(wld, p->location, arg, dest);
	if (dest[0] == '\0')
		return ((void)write(p->sd, "ERR 301 NO_EXIT\n", 16));
	if (ai_core_locked(p, dest))
		return ;
	sprintf(out, "EVT ROOM PRESENCE LEAVE %s\n", p->name);
	broadcast_room(c, p->location, out, p->sd);
	strncpy(p->last_location, p->location, 31);
	strncpy(p->location, dest, 31);
	p->location[31] = '\0';
	p->in_combat = 0;
	sprintf(out, "EVT ROOM PRESENCE ENTER %s\n", p->name);
	broadcast_room(c, p->location, out, p->sd);
	server_log("INFO", p->ip, p->name, "Moved to a new room");
	sprintf(out, "OK room=%s\n", p->location);
	write(p->sd, out, strlen(out));
}
