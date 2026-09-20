/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_group_who.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 17:11:42 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/19 14:11:20 by ayhammou         ###   ########.fr       */
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

static void	group_join(t_player *p, t_player *c, t_player *leader)
{
	char	o[128];

	if (p->group_id != 0)
		return ((void)write(p->sd, "ERR 402 ALREADY_IN_GROUP\n", 25));
	if (leader->group_id == 0)
		return ((void)write(p->sd, "ERR 401 NOT_IN_GROUP\n", 21));
	p->group_id = leader->group_id;
	sprintf(o, "EVT GROUP JOIN %s\n", p->name);
	broadcast_group(c, p->group_id, o, p->sd);
	server_log("INFO", p->ip, p->name, "Joined a group");
	sprintf(o, "OK group=%d\n", p->group_id);
	write(p->sd, o, strlen(o));
}

static void	group_leave(t_player *p, t_player *c)
{
	char	o[128];

	sprintf(o, "EVT GROUP LEAVE %s\n", p->name);
	broadcast_group(c, p->group_id, o, p->sd);
	p->group_id = 0;
	server_log("INFO", p->ip, p->name, "Left the group");
	write(p->sd, "OK\n", 3);
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
		if (!strcmp(a, "JOIN"))
			return (group_join(p, c, &c[i]));
	}
	write(p->sd, "ERR 410 PLAYER_NOT_FOUND\n", 25);
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
		return (group_leave(p, c));
	if (ac == 2)
		return (handle_group_target(p, c, a, t));
	write(p->sd, "ERR 400 BAD_REQUEST\n", 20);
}
