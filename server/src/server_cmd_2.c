/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 11:40:23 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/02 11:41:21 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_look(t_player *player, char *world_data)
{
	char	search_key[64];
	char	room_name[64];
	char	response[256];

	sprintf(search_key, "\"%s\": {\n        \"name\":", player->location);
	extract_json_value(world_data, search_key, room_name);

	if (strlen(room_name) > 0)
	{
		sprintf(response, "Salle actuelle : %s\n", room_name);
		write(player->sd, response, strlen(response));
	}
	else
		write(player->sd, "ERR room not found\n", 19);
}

void	handle_move(t_player *player, int words, char *arg, char *world)
{
	char	dest[32];
	char	response[64];

	if (words != 2)
	{
		write(player->sd, "ERR missing direction\n", 22);
		return ;
	}

	get_room_exit(world, player->location, arg, dest);

	if (strlen(dest) > 0)
	{
		strncpy(player->location, dest, 31);
		player->location[31] = '\0';

		printf("Player %s moved to : %s\n", player->name, dest);
		sprintf(response, "OK moved to %s\n", dest);
		write(player->sd, response, strlen(response));
	}
	else
	{
		write(player->sd, "ERR invalid direction\n", 22);
	}
}
