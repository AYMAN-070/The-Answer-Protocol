/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_manage_dropped.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 14:55:03 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/11 18:09:59 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

char	*manage_dropped(int mode, char *room, char *item, char *buf)
{
	static t_drop	d[50];
	int				i;

	i = -1;
	while (++i < 50)
	{
		if (mode == 1 && d[i].room[0] == '\0')
		{
			strncpy(d[i].room, room, 31);
			strncpy(d[i].item, item, 31);
			return ("OK");
		}
		else if (mode == 2 && !strcmp(d[i].room, room)
			&& !strcmp(d[i].item, item))
		{
			d[i].room[0] = '\0';
			return ("OK");
		}
		else if (mode == 3 && buf && !strcmp(d[i].room, room))
		{
			strcat(buf, d[i].item);
			strcat(buf, ",");
		}
	}
	return (NULL);
}
