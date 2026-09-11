/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 20:08:00 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/11 18:14:00 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_connect(t_player *player, int words, char *arg)
{
	int	i;

	if (words == 2)
	{
		strncpy(player->name, arg, 31);
		player->name[31] = '\0';
		strncpy(player->location, "airlock", 31);
		strncpy(player->last_location, "airlock", 31);
		player->location[31] = '\0';
		player->hp = 100;
		player->max_hp = 100;
		player->in_combat = 0;
		i = 0;
		while (i < 5)
		{
			player->inventory[i][0] = '\0';
			i++;
		}
		server_log("INFO", player->ip, player->name, "Connected to server");
		write(player->sd, "OK connected\n", 13);
	}
	else
		write(player->sd, "ERR missing name\n", 17);
}

void	handle_who(t_player *player, t_player *clients)
{
	int		i;
	char	msg[512];
	char	line[128];

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

void	handle_inventory(t_player *player)
{
	char	msg[256];
	int		i;
	int		empty;

	strcpy(msg, "INVENTORY:");
	i = 0;
	empty = 1;
	while (i < 5)
	{
		if (strlen(player->inventory[i]) > 0)
		{
			if (empty == 0)
				strcat(msg, ",");
			strcat(msg, player->inventory[i]);
			empty = 0;
		}
		i++;
	}
	if (empty == 1)
		strcat(msg, "empty");
	strcat(msg, "\n");
	write(player->sd, msg, strlen(msg));
}

int	process_combat_cmd(t_player *p, t_player *c, char *cmd, char *arg, char *wld)
{
	int	w;

	w = 1;
	if (arg[0] != '\0')
		w = 2;
	if (strcmp(cmd, "ATTACK") == 0)
		handle_attack(p, w, arg, wld);
	else if (strcmp(cmd, "DEFEND") == 0)
		handle_defend(p, w, arg, wld);
	else if (strcmp(cmd, "FLEE") == 0)
		handle_flee(p, wld);
	else if (strcmp(cmd, "DROP") == 0)
		handle_drop(p, w, arg, wld);
	else if (strcmp(cmd, "TAKE") == 0)
		handle_take(p, w, arg, wld);
	else if (strcmp(cmd, "TALK") == 0)
		handle_talk(p, w, arg, wld);
	else if (strcmp(cmd, "STATUS") == 0)
		handle_status(p);
	else if (strcmp(cmd, "QUEST") == 0 || strcmp(cmd, "QUESTS") == 0)
		handle_quest(p);
	else if (w >= 1 && (strcmp(cmd, "RCHAT") == 0 || strcmp(cmd, "GCHAT") == 0))
		handle_chat(p, c, cmd, arg);
	else
		return (0);
	return (1);
}

void	process_command(t_player *p, t_player *c, char *b, char *wld)
{
	char	cmd[32];
	char	arg[512];
	int		w;

	if (check_abuse(p) == 1)
		return ;
	memset(cmd, 0, 32);
	memset(arg, 0, 512);
	w = sscanf(b, "%31s %[^\n]", cmd, arg);
	if (w >= 1)
		server_log("INFO", p->ip, p->name, cmd);
	if (w >= 1 && strcmp(cmd, "CONNECT") == 0)
		handle_connect(p, w, arg);
	else if (w >= 1 && strcmp(cmd, "LOOK") == 0)
		handle_look(p, wld);
	else if (w >= 1 && strcmp(cmd, "MOVE") == 0)
		handle_move(p, w, arg, wld);
	else if (w >= 1 && strcmp(cmd, "WHO") == 0)
		handle_who(p, c);
	else if (w >= 1 && strcmp(cmd, "INVENTORY") == 0)
		handle_inventory(p);
	else if (w >= 1 && strcmp(cmd, "QUIT") == 0)
		handle_quit(p);
	else if (process_combat_cmd(p, c, cmd, arg, wld) == 0)
		write(p->sd, "ERR unknown command\n", 20);
}
