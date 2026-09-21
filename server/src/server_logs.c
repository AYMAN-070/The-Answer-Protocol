/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_logs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 20:56:31 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/09 23:16:46 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <time.h>
#include <stdio.h>

static void	escape_json(char *out, char *in, int max)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (in[i] && j < max - 2)
	{
		if (in[i] == '"' || in[i] == '\\')
			out[j++] = '\\';
		if ((unsigned char)in[i] >= 32)
			out[j++] = in[i];
		i++;
	}
	out[j] = '\0';
}

void	server_log(char *level, char *ip, char *player, char *msg)
{
	time_t		t;
	struct tm	*tm_info;
	char		time_str[32];
	char		safe[512];
	char		who[64];

	time(&t);
	tm_info = localtime(&t);
	strftime(time_str, 32, "%Y-%m-%dT%H:%M:%S", tm_info);
	escape_json(safe, msg, 512);
	escape_json(who, player, 64);
	printf("{\"time\":\"%s\", \"lvl\":\"%s\", \"ip\":\"%s\", "
		"\"player\":\"%s\", \"msg\":\"%s\"}\n",
		time_str, level, ip, who, safe);
}

void	send_reply(t_player *p, char *msg)
{
	char	buf[256];
	int		i;

	i = 0;
	while (msg[i] && msg[i] != '\n' && i < 200)
	{
		buf[i] = msg[i];
		i++;
	}
	buf[i] = '\0';
	if (strncmp(msg, "ERR", 3) == 0)
		server_log("WARN", p->ip, p->name, buf);
	else
		server_log("INFO", p->ip, p->name, buf);
	write(p->sd, msg, strlen(msg));
}

int	check_abuse(t_player *player)
{
	time_t	now;

	now = time(NULL);
	if (now == player->last_cmd_time)
		player->cmd_count++;
	else
	{
		player->last_cmd_time = now;
		player->cmd_count = 1;
	}
	if (player->cmd_count > 5)
	{
		server_log("WARN", player->ip, player->name, "Command flood detected");
		return (1);
	}
	return (0);
}
