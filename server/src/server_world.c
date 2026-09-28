/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_world.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:30:52 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/28 09:40:56 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

int	world_state(int mode, char *key)
{
	static char	flags[8][32];
	int			i;

	i = -1;
	while (++i < 8)
	{
		if (strcmp(flags[i], key) == 0)
			return (1);
	}
	if (mode == 1)
		return (0);
	i = -1;
	while (++i < 8)
	{
		if (flags[i][0] == '\0')
		{
			strncpy(flags[i], key, 31);
			flags[i][31] = '\0';
			return (1);
		}
	}
	return (0);
}

static int	room_exists(char *wld, char *room)
{
	char	key[64];

	if (room[0] == '\0')
		return (1);
	sprintf(key, "\"%s\": {", room);
	if (!strstr(wld, key))
		return (0);
	return (1);
}

int	check_room_exits(char *wld, char *room)
{
	char	*dir[4];
	char	dest[64];
	int		i;
	int		ok;

	dir[0] = "north";
	dir[1] = "south";
	dir[2] = "east";
	dir[3] = "west";
	ok = 1;
	i = -1;
	while (++i < 4)
	{
		get_room_exit(wld, room, dir[i], dest);
		if (!room_exists(wld, dest))
		{
			server_log("ERROR", "sys", room, "Exit points to unknown room");
			ok = 0;
		}
	}
	return (ok);
}
