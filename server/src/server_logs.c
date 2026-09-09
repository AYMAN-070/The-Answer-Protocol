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

void	server_log(char *level, char *ip, char *player, char *msg)
{
	time_t		t;
	struct tm	*tm_info;
	char		time_str[32];

	time(&t);
	tm_info = localtime(&t);
	strftime(time_str, 32, "%Y-%m-%dT%H:%M:%S", tm_info);
	printf("{\"time\":\"%s\", \"lvl\":\"%s\", \"ip\":\"%s\", "
		"\"player\":\"%s\", \"msg\":\"%s\"}\n",
		time_str, level, ip, player, msg);
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
