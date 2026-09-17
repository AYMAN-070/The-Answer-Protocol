/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_group_who.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 17:11:42 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/17 13:32:47 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

void	handle_who(t_player *p, t_player *clients)
{
	int		i;
	int		count;
	char	msg[32];

	count = 0;
	i = -1;
	while (++i < 10)
	{
		if (clients[i].is_active == 1)
			count++;
	}
	sprintf(msg, "OK players=%d\n", count);
	write(p->sd, msg, strlen(msg));
}

static void	handle_group_target(t_player *p, t_player *c, char *act, char *tgt)
{
	int		i;
	char	out[128];

	i = -1;
	while (++i < 10)
	{
		if (c[i].is_active == 1 && strcmp(c[i].name, tgt) == 0)
		{
			if (strcmp(act, "INVITE") == 0)
			{
				sprintf(out, "EVT GROUP INVITE %s\n", p->name);
				write(c[i].sd, out, strlen(out));
				return ((void)write(p->sd, "OK Invited\n", 11));
			}
			else if (strcmp(act, "JOIN") == 0)
			{
				if (p->group_id != 0)
				{
					write(p->sd, "ERR 402 ALREADY_IN_GROUP\n", 25);
					return ;
				}
				p->group_id = c[i].group_id;
				sprintf(out, "OK group=%d\n", p->group_id);
				return ((void)write(p->sd, out, strlen(out)));
			}
		}
	}
	write(p->sd, "ERR player not found\n", 21);
}

void	handle_group(t_player *p, t_player *c, char *arg)
{
	char		act[32];
	char		tgt[32];
	char		out[64];
	static int	g_id = 1;

	act[0] = '\0';
	tgt[0] = '\0';
	sscanf(arg, "%31s %31s", act, tgt);
	if (strcmp(act, "CREATE") == 0)
	{
		if (p->group_id != 0)
			return ((void)write(p->sd, "ERR 402 ALREADY_IN_GROUP\n", 25));
		p->group_id = g_id++;
		sprintf(out, "OK group=%d\n", p->group_id);
		write(p->sd, out, strlen(out));
	}
	else if (strcmp(act, "LEAVE") == 0)
	{
		if (p->group_id == 0)
			return ((void)write(p->sd, "ERR 401 NOT_IN_GROUP\n", 21));
		p->group_id = 0;
		write(p->sd, "OK\n", 3);
	}
	else if (tgt[0] != '\0')
		handle_group_target(p, c, act, tgt);
	else
		write(p->sd, "ERR missing argument\n", 21);
}