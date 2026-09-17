/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 19:07:59 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/17 17:19:56 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_inventory(t_player *p)
{
	char	msg[256];
	int		i;
	int		first;

	strcpy(msg, "OK [");
	first = 1;
	i = -1;
	while (++i < 5)
	{
		if (p->inventory[i][0] != '\0')
		{
			if (!first)
				strcat(msg, ", ");
			strcat(msg, "\"");
			strcat(msg, p->inventory[i]);
			strcat(msg, "\"");
			first = 0;
		}
	}
	strcat(msg, "]\n");
	write(p->sd, msg, strlen(msg));
}

void handle_drop(t_player *player, t_player *c, char *item, char *world_data)
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

char	*manage_taken(int mode, char *item)
{
	static char	taken[128][32] = {0};
	int			i;

	i = -1;
	if (mode == 1)
	{
		while (++i < 128)
			if (strcmp(taken[i], item) == 0)
				return (taken[i]);
	}
	else if (mode == 2)
	{
		while (++i < 128)
			if (taken[i][0] == '\0')
			{
				strncpy(taken[i], item, 31);
				return (taken[i]);
			}
	}
	return (NULL);
}

static int	check_take_err(t_player *p, int w, char *item, char *world)
{
	if (w != 2)
	{
		server_log("WARN", p->ip, p->name, "Take missing item");
		return (write(p->sd, "ERR missing item\n", 17), 1);
	}
	if (has_item(p, item) == 1)
	{
		server_log("WARN", p->ip, p->name, "Take duplicate item");
		return (write(p->sd, "ERR you already have this\n", 26), 1);
	}
	if ((check_item_in_room(world, p->location, item) == 0
			|| manage_taken(1, item) != NULL)
		&& manage_dropped(2, p->location, item, NULL) == NULL)
	{
		server_log("WARN", p->ip, p->name, "Take item not available");
		return (write(p->sd, "ERR 404 ITEM_NOT_FOUND\n", 23), 1);
	}
	return (0);
}

void	handle_take(t_player *player, int words, char *item, char *world)
{
	int		i;
	char	out[64];

	if (check_take_err(player, words, item, world) == 1)
		return ;
	i = -1;
	while (++i < 5)
	{
		if (player->inventory[i][0] == '\0')
		{
			strncpy(player->inventory[i], item, 31);
			server_log("INFO", player->ip, player->name, "Item taken");
			sprintf(out, "OK taken=%s\n", item);
			write(player->sd, out, strlen(out));
			manage_taken(2, item);
			return ;
		}
	}
	server_log("WARN", player->ip, player->name, "Inventory Full");
	write(player->sd, "ERR inventory full\n", 19);
}
