/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_group_who.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 17:11:42 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/18 23:54:18 by ayhammou         ###   ########.fr       */
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

static void	handle_group_target(t_player *p, t_player *c, char *a, char *tgt)
{
	char	o[128];
	int		i;

	i = -1;
	while (++i < 10)
	{
		if (!c[i].is_active || strcmp(c[i].name, tgt))
			continue ;
		if (!strcmp(a, "INVITE"))
		{
			sprintf(o, "EVT GROUP INVITE %s\n", p->name);
			write(c[i].sd, o, strlen(o));
			return ((void)write(p->sd, "OK Invited\n", 11));
		}
		if (!strcmp(a, "JOIN") && p->group_id != 0)
			return ((void)write(p->sd, "ERR 402 ALREADY_IN_GROUP\n", 25));
		if (!strcmp(a, "JOIN"))
		{
			p->group_id = c[i].group_id;
			sprintf(o, "OK group=%d\n", p->group_id);
			return ((void)write(p->sd, o, strlen(o)));
		}
	}
	write(p->sd, "ERR player not found\n", 21);
}

void	handle_group(t_player *p, t_player *c, char *arg)
{
	static int	g = 1;
	char		a[32];
	char		t[32];
	char		o[64];
	int			ac;

	ac = sscanf(arg, "%31s %31s", a, t);
	if (ac > 0 && !strcmp(a, "CREATE") && p->group_id != 0)
		return ((void)write(p->sd, "ERR 402 ALREADY_IN_GROUP\n", 25));
	if (ac > 0 && !strcmp(a, "CREATE"))
	{
		p->group_id = g++;
		sprintf(o, "OK group=%d\n", p->group_id);
		return ((void)write(p->sd, o, strlen(o)));
	}
	if (ac > 0 && !strcmp(a, "LEAVE") && p->group_id == 0)
		return ((void)write(p->sd, "ERR 401 NOT_IN_GROUP\n", 21));
	if (ac > 0 && !strcmp(a, "LEAVE"))
	{
		p->group_id = 0;
		return ((void)write(p->sd, "OK\n", 3));
	}
	if (ac == 2)
		return (handle_group_target(p, c, a, t));
	write(p->sd, "ERR missing argument\n", 21);
}
