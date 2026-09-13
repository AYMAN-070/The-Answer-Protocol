/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 11:40:23 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/13 18:08:56 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_look(t_player *player, char *world_data)
{
	char	search_key[64];
	char	room_name[64];
	char	response[256];
	char	dropped_msg[256];

	sprintf(search_key, "\"%s\": {\n        \"name\":", player->location);
	extract_json_value(world_data, search_key, room_name);
	if (strlen(room_name) > 0)
	{
		sprintf(response, "Current Room : %s\n", room_name);
		server_log("INFO", player->ip, player->name, "Look successful");
		write(player->sd, response, strlen(response));
		strcpy(dropped_msg, "DROPPED:");
		manage_dropped(3, player->location, NULL, dropped_msg);
		strcat(dropped_msg, "\n");
		write(player->sd, dropped_msg, strlen(dropped_msg));
	}
	else
	{
		server_log("ERROR", player->ip, player->name, "Look room not found");
		write(player->sd, "ERR room not found\n", 19);
	}
}

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

void	handle_move(t_player *player, int words, char *arg, char *world)
{
	char	dest[32];

	if (words != 2)
	{
		write(player->sd, "ERR missing direction\n", 22);
		return ;
	}
	get_room_exit(world, player->location, arg, dest);
	if (strlen(dest) == 0)
	{
		server_log("WARN", player->ip, player->name, "Invalid direction");
		write(player->sd, "ERR invalid direction\n", 22);
		return ;
	}
	if (strcmp(dest, "ai_core") == 0 && has_item(player, "red_keycard") == 0)
	{
		server_log("WARN", player->ip, player->name,
			"Access denied: missing keycard");
		write(player->sd, "ERR access denied : red_keycard required\n", 41);
		return ;
	}
	strncpy(player->last_location, player->location, 31);
	strncpy(player->location, dest, 31);
	player->location[31] = '\0';
	handle_look(player, world);
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
	if (check_item_in_room(world, p->location, item) == 0
		&& manage_dropped(2, p->location, item, NULL) == NULL)
	{
		server_log("WARN", p->ip, p->name, "Take item not here");
		return (write(p->sd, "ERR item not here\n", 18), 1);
	}
	return (0);
}

void	handle_take(t_player *player, int words, char *item, char *world)
{
	int	i;

	if (check_take_err(player, words, item, world) == 1)
		return ;
	i = -1;
	while (++i < 5)
	{
		if (player->inventory[i][0] == '\0')
		{
			strncpy(player->inventory[i], item, 31);
			server_log("INFO", player->ip, player->name, "Item taken");
			write(player->sd, "OK item taken\n", 14);
			handle_inventory(player);
			return ;
		}
	}
	server_log("WARN", player->ip, player->name, "Inventory Full");
	write(player->sd, "ERR inventory full\n", 19);
}
