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

static void	broadcast_room(t_player *c, char *room, char *msg, int exclude_sd)
{
	int	i;

	i = -1;
	while (++i < 10)
	{
		if (c[i].is_active && c[i].sd != exclude_sd
			&& strcmp(c[i].location, room) == 0)
			write(c[i].sd, msg, strlen(msg));
	}
}

void	handle_move(t_player *p, t_player *c, char *arg, char *wld)
{
	char	dest[32];
	char	out[128];

	dest[0] = '\0';
	if (arg[0] == '\0')
		return ((void)write(p->sd, "ERR missing direction\n", 22));
	get_room_exit(wld, p->location, arg, dest);
	if (dest[0] == '\0')
		return ((void)write(p->sd, "ERR invalid direction\n", 22));
	if (strcmp(dest, "ai_core") == 0 && has_item(p, "red_keycard") == 0)
		return ((void)write(p->sd,
				"ERR access denied : red_keycard required\n", 41));
	sprintf(out, "EVT ROOM PRESENCE LEAVE %s\n", p->name);
	broadcast_room(c, p->location, out, p->sd);
	strncpy(p->last_location, p->location, 31);
	strncpy(p->location, dest, 31);
	p->location[31] = '\0';
	sprintf(out, "EVT ROOM PRESENCE ENTER %s\n", p->name);
	broadcast_room(c, p->location, out, p->sd);
	handle_look(p, c, wld);
}
