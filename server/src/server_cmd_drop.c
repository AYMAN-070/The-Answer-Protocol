/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_drop.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 14:55:03 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/18 23:49:09 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

char	*manage_dropped(int mode, char *room, char *item, char *buf)
{
	static t_drop	d[50];
	int				i;

	i = -1;
	while (++i < 50)
	{
		if (mode == 1 && d[i].room[0] == '\0')
		{
			strncpy(d[i].room, room, 31);
			strncpy(d[i].item, item, 31);
			return ("OK");
		}
		if (mode == 2 && !strcmp(d[i].room, room) && !strcmp(d[i].item, item))
		{
			d[i].room[0] = '\0';
			return ("OK");
		}
		if (mode == 3 && buf && !strcmp(d[i].room, room) && strlen(buf) < 200)
		{
			if (buf[0] != '\0')
				strcat(buf, ", ");
			sprintf(buf + strlen(buf), "\"%s\"", d[i].item);
		}
	}
	return (NULL);
}

void	handle_drop(t_player *player, char *item, char *world_data)
{
	int		i;
	char	id[32];
	char	msg[128];

	if (item == NULL || item[0] == '\0')
		return ((void)write(player->sd, "ERR 404 ITEM_NOT_IN_INVENTORY\n", 30));
	resolve_id(world_data, "\"items\": {", item, id);
	i = -1;
	while (++i < 5)
	{
		if (strcmp(player->inventory[i], id) == 0)
		{
			player->inventory[i][0] = '\0';
			manage_dropped(1, player->location, id, NULL);
			server_log("INFO", player->ip, player->name, "Item dropped");
			sprintf(msg, "OK dropped=%s\n", id);
			return ((void)write(player->sd, msg, strlen(msg)));
		}
	}
	server_log("WARN", player->ip, player->name, "Drop item not in inventory");
	write(player->sd, "ERR 404 ITEM_NOT_IN_INVENTORY\n", 30);
}
