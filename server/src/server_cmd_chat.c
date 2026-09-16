/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_chat.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 17:39:36 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/13 18:43:46 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_chat(t_player *player, t_player *clients, char *cmd, char *msg)
{
	int		i;
	char	out[512];
	int		is_global;

	if (!msg || strlen(msg) == 0)
		return ;
	is_global = (strcmp(cmd, "GCHAT") == 0);
	sprintf(out, "%s:%s: %s\n", cmd, player->name, msg);
	i = -1;
	while (++i < 10)
	{
		if (clients[i].is_active == 1 && clients[i].sd != player->sd)
		{
			if (is_global || strcmp(clients[i].location, player->location) == 0)
				write(clients[i].sd, out, strlen(out));
		}
	}
	server_log("INFO", player->ip, player->name, "Chat message sent");
	write(player->sd, "OK\n", 3);
}
