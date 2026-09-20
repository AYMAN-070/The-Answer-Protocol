/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_take_inventory.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 19:07:59 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/18 23:53:02 by ayhammou         ###   ########.fr       */
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

char	*manage_taken(int mode, char *item)
{
	static char	t[128][32];
	int			i;

	i = -1;
	if (mode == 1)
	{
		while (++i < 128)
			if (!strcmp(t[i], item))
				return (t[i]);
	}
	if (mode == 2)
	{
		while (++i < 128)
		{
			if (t[i][0] == '\0')
			{
				strncpy(t[i], item, 31);
				return (t[i]);
			}
		}
	}
	return (NULL);
}

static int	check_take_err(t_player *p, int w, char *item, char *wld)
{
	if (w != 2)
	{
		server_log("WARN", p->ip, p->name, "Take missing item");
		write(p->sd, "ERR 404 ITEM_NOT_FOUND\n", 23);
		return (1);
	}
	if (has_item(p, item) == 1)
	{
		server_log("WARN", p->ip, p->name, "Take duplicate item");
		write(p->sd, "ERR 404 ITEM_NOT_FOUND\n", 23);
		return (1);
	}
	if ((!check_item_in_room(wld, p->location, item) || manage_taken(1, item))
		&& !manage_dropped(2, p->location, item, NULL))
	{
		server_log("WARN", p->ip, p->name, "Take item not available");
		write(p->sd, "ERR 404 ITEM_NOT_FOUND\n", 23);
		return (1);
	}
	return (0);
}

static void	take_item_slot(t_player *p, char *id, int i)
{
	char	out[64];

	strncpy(p->inventory[i], id, 31);
	p->inventory[i][31] = '\0';
	manage_taken(2, id);
	server_log("INFO", p->ip, p->name, "Item taken");
	sprintf(out, "OK taken=%s\n", id);
	write(p->sd, out, strlen(out));
}

void	handle_take(t_player *player, int words, char *item, char *world)
{
	int		i;
	char	id[32];

	id[0] = '\0';
	if (words == 2)
		resolve_id(world, "\"items\": {", item, id);
	if (check_take_err(player, words, id, world) == 1)
		return ;
	if (!is_item_obtainable(world, id))
		return ((void)write(player->sd, "ERR 407 ITEM_NOT_OBTAINABLE\n", 28));
	i = -1;
	while (++i < 5)
	{
		if (player->inventory[i][0] == '\0')
			return (take_item_slot(player, id, i));
	}
	server_log("WARN", player->ip, player->name, "Inventory Full");
	write(player->sd, "ERR 408 INVENTORY_FULL\n", 23);
}
