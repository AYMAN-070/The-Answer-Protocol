/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 20:08:00 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/02 11:41:25 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"


void	handle_quit(t_player *player)
{
	printf("PLayer %s (socket %d) disconnected.\n", player->name, player->sd);
	write(player->sd, "OK goodbye\n", 11);
}

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

void	handle_who(t_player *player, t_player *clients)
{
	int		i;
	char	msg[512];
	char	line[64];

	strcpy(msg, "Joueurs en ligne :\n");
	i = 0;
	while (i < 10)
	{
		if (clients[i].is_active == 1)
		{
			sprintf(line, "- %s (Salle: %s)\n",
				clients[i].name, clients[i].location);
			strcat(msg, line);
		}
		i++;
	}
	write(player->sd, msg, strlen(msg));
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
