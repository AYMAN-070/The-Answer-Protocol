/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_chat.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 17:39:36 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/17 15:44:25 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_chat(t_player *p, t_player *c, char *arg)
{
	char	scope[16];
	char	msg[1024];
	char	out[2048];
	int		i;

	scope[0] = '\0';
	msg[0] = '\0';
	sscanf(arg, "%15s %[^\n]", scope, msg);
	if (scope[0] == '\0' || msg[0] == '\0')
		return ((void)write(p->sd, "ERR missing argument\n", 21));
	if (strcmp(scope, "ROOM") == 0)
		snprintf(out, sizeof(out), "EVT ROOM CHAT %s %s\n", p->name, msg);
	else if (strcmp(scope, "GLOBAL") == 0)
		snprintf(out, sizeof(out), "EVT GLOBAL CHAT %s %s\n", p->name, msg);
	else
		return ;
	i = -1;
	while (++i < 10)
	{
		if (c[i].is_active && c[i].sd != p->sd)
		{
			if (strcmp(scope, "ROOM") == 0
				&& strcmp(c[i].location, p->location) == 0)
				write(c[i].sd, out, strlen(out));
			else if (strcmp(scope, "GLOBAL") == 0)
				write(c[i].sd, out, strlen(out));
		}
	}
	write(p->sd, "OK\n", 3);
}
