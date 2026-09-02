/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 20:08:00 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/02 11:24:56 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_connect(t_player *player, int words, char *arg)
{
	if (words == 2)
	{
		strncpy(player->name, arg, 31);
		player->name[31] = '\0';
		strncpy(player->location, "airlock", 31);
		player->location[31] = '\0';
		printf("Player (socket %d) is called : %s\n", player->sd, player->name);
		write(player->sd, "OK connected\n", 13);
	}
	else
	{
		printf("Error socket %d : missing name to CONNECT.\n", player->sd);
		write(player->sd, "ERR missing name\n", 17);
	}
}

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

void	handle_quit(t_player *player)
{
	printf("PLayer %s (socket %d) disconnected.\n", player->name, player->sd);
	write(player->sd, "OK goodbye\n", 11);
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

void	handle_who(t_player *player, t_player *clients)
{
	int		i;
	char	msg[128];

	write(player->sd, "Plauer online:\n", 19);
	i = 0;
	while (i < 10)
	{
		if (clients[i].is_active == 1)
		{
			sprintf(msg, "- %s (Salle: %s)\n", clients[i].name, clients[i].location);
			write(player->sd, msg, strlen(msg));
		}
		i++;
	}
}

void	process_command(t_player *player, t_player *clients,
	char *buffer, char *world_data)
{
	char	cmd[32];
	char	arg[32];
	int		words;

	memset(cmd, 0, 32);
	memset(arg, 0, 32);
	words = sscanf(buffer, "%31s %31s", cmd, arg);
	if (words >= 1 && strcmp(cmd, "CONNECT") == 0)
		handle_connect(player, words, arg);
	else if (words >= 1 && strcmp(cmd, "LOOK") == 0)
		handle_look(player, world_data);
	else if (words >= 1 && strcmp(cmd, "MOVE") == 0)
		handle_move(player, words, arg, world_data);
	else if (words >= 1 && strcmp(cmd, "WHO") == 0)
		handle_who(player, clients);
	else if (words >= 1 && strcmp(cmd, "QUIT") == 0)
		handle_quit(player);
	else
	{
		printf("Error socket %d : Unknown Commande.\n", player->sd);
		write(player->sd, "ERR unknown command\n", 20);
	}
}
