/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_chat.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 17:39:36 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/18 23:44:11 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static void	chat_broadcast(t_player *p, t_player *c, char *s, char *o)
{
	int	i;

	i = -1;
	while (++i < 10)
	{
		if (c[i].is_active && c[i].sd != p->sd)
		{
			if (!strcmp(s, "ROOM") && !strcmp(c[i].location, p->location))
				write(c[i].sd, o, strlen(o));
			else if (!strcmp(s, "GLOBAL"))
				write(c[i].sd, o, strlen(o));
			else if (!strcmp(s, "GROUP") && c[i].group_id == p->group_id)
				write(c[i].sd, o, strlen(o));
		}
	}
}

void	handle_chat(t_player *p, t_player *c, char *arg)
{
	char	s[16];
	char	m[1024];
	char	o[2048];

	if (sscanf(arg, "%15s %[^\n]", s, m) < 2)
		return ((void)write(p->sd, "ERR 400 BAD_REQUEST\n", 20));
	if (strcmp(s, "ROOM") && strcmp(s, "GLOBAL") && strcmp(s, "GROUP"))
		return ((void)write(p->sd, "ERR 400 BAD_REQUEST\n", 20));
	if (!strcmp(s, "GROUP") && p->group_id == 0)
		return ((void)write(p->sd, "ERR 401 NOT_IN_GROUP\n", 21));
	snprintf(o, sizeof(o), "EVT %s CHAT %s %s\n", s, p->name, m);
	chat_broadcast(p, c, s, o);
	write(p->sd, "OK\n", 3);
}
