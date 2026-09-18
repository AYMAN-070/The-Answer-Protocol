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
		if (mode == 3 && buf && !strcmp(d[i].room, room))
		{
			if (buf[0] != '\0')
				strcat(buf, ", ");
			sprintf(buf + strlen(buf), "\"%s\"", d[i].item);
		}
	}
	return (NULL);
}

void	handle_drop(t_player *player, t_player *c, char *item, char *world_data)
{
	int		i;
	char	msg[128];

	if (item == NULL || item[0] == '\0')
	{
		server_log("WARN", player->ip, player->name, "Drop missing item");
		return ((void)write(player->sd, "ERR missing item\n", 17));
	}
	i = -1;
	while (++i < 5)
	{
		if (strcmp(player->inventory[i], item) == 0)
		{
			player->inventory[i][0] = '\0';
			manage_dropped(1, player->location, item, NULL);
			server_log("INFO", player->ip, player->name, "Item dropped");
			sprintf(msg, "OK dropped=%s\n", item);
			write(player->sd, msg, strlen(msg));
			handle_inventory(player);
			handle_look(player, c, world_data);
			return ;
		}
	}
	server_log("WARN", player->ip, player->name, "Drop item not found");
	write(player->sd, "ERR item not found\n", 19);
}
