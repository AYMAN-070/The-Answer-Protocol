/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 20:08:00 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/18 19:57:32 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static void	init_player_data(t_player *p, char *arg)
{
	int	i;

	strncpy(p->name, arg, 31);
	p->name[31] = '\0';
	strncpy(p->location, "airlock", 31);
	strncpy(p->last_location, "airlock", 31);
	p->location[31] = '\0';
	p->hp = 100;
	p->max_hp = 100;
	p->in_combat = 0;
	p->group_id = 0;
	p->quest_status = 0;
	p->quest_override = 0;
	i = -1;
	while (++i < 5)
		p->inventory[i][0] = '\0';
}

void	handle_connect(t_player *p, t_player *c, int words, char *arg)
{
	int	i;

	if (words < 2 || arg[0] == '\0')
		return ((void)write(p->sd, "ERR missing name\n", 17));
	i = -1;
	while (++i < 10)
	{
		if (c[i].is_active == 1 && strcmp(c[i].name, arg) == 0)
			return ((void)write(p->sd, "ERR 201 NAME_IN_USE\n", 20));
	}
	init_player_data(p, arg);
	server_log("INFO", p->ip, p->name, "Connected to server");
	write(p->sd, "OK connected\n", 13);
	broadcast_player_stats(c);
}

int	process_combat_cmd(t_player *p, t_player *c, char *buf, char *wld)
{
	char	cmd[32];
	char	arg[512];
	int		w;

	cmd[0] = '\0';
	arg[0] = '\0';
	w = 1 + (sscanf(buf, "%31s %[^\n]", cmd, arg) > 1);
	if (strcmp(cmd, "ATTACK") == 0)
		handle_attack(p, c, arg, wld);
	else if (strcmp(cmd, "DEFEND") == 0)
		handle_defend(p, c, arg, wld);
	else if (strcmp(cmd, "TAKE") == 0)
		handle_take(p, w, arg, wld);
	else if (strcmp(cmd, "TALK") == 0)
		handle_talk(p, w, arg, wld);
	else if (strcmp(cmd, "QUEST") == 0)
		handle_quest(p, arg, wld);
	else
		return (0);
	return (1);
}

static int	exec_other_cmds(t_player *p, t_player *c, char *cmd, char *arg)
{
	if (strcmp(cmd, "WHO") == 0)
		handle_who(p, c);
	else if (strcmp(cmd, "INVENTORY") == 0)
		handle_inventory(p);
	else if (strcmp(cmd, "STATUS") == 0)
		handle_status(p);
	else if (strcmp(cmd, "GROUP") == 0)
		handle_group(p, c, arg);
	else if (strcmp(cmd, "CHAT") == 0)
		handle_chat(p, c, arg);
	else if (strcmp(cmd, "QUIT") == 0)
		handle_quit(p, c);
	else if (strcmp(cmd, "QUESTS") == 0)
		handle_quests(p);
	else
		return (0);
	return (1);
}

void	process_command(t_player *p, t_player *c, char *b, char *wld)
{
	char	cmd[32];
	char	arg[512];

	cmd[0] = '\0';
	arg[0] = '\0';
	if (check_abuse(p) == 1 || sscanf(b, "%31s %[^\n]", cmd, arg) < 1)
		return ((void)write(p->sd, "ERR ignored\n", 12));
	server_log("INFO", p->ip, p->name, cmd);
	printf("DEBUG - Commande reçue du client : [%s]\n", b);
	if (strcmp(cmd, "CONNECT") == 0)
		handle_connect(p, c, 1 + (arg[0] != '\0'), arg);
	else if (strcmp(cmd, "LOOK") == 0)
		handle_look(p, c, wld);
	else if (strcmp(cmd, "MOVE") == 0)
		handle_move(p, c, arg, wld);
	else if (strcmp(cmd, "FLEE") == 0)
		handle_flee(p, c, wld);
	else if (strcmp(cmd, "DROP") == 0)
		handle_drop(p, c, arg, wld);
	else if (exec_other_cmds(p, c, cmd, arg) == 0 
		&& process_combat_cmd(p, c, b, wld) == 0)
	{
		write(p->sd, "ERR unknown command\n", 20);
	}
}
