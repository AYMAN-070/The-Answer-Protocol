/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_world.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:30:52 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/09 22:31:27 by ayhammou         ###   ########.fr       */
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

static int	check_room_exits(char *wld, char *room)
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

void	validate_world(char *wld)
{
	char	*end;
	char	*p;
	char	*k;
	char	room[64];

	end = strstr(wld, "\"items\": {");
	p = strstr(wld, "\"locations\": {");
	if (!p)
		exit_error("Error : no locations in world.json");
	p = strstr(p + 14, "\": {");
	while (p && (!end || p < end))
	{
		k = p;
		while (k > wld && *(k - 1) != '"')
			k--;
		if ((p - k) < 63)
		{
			strncpy(room, k, p - k);
			room[p - k] = '\0';
			if (!check_room_exits(wld, room))
				exit(EXIT_FAILURE);
		}
		p = strstr(p + 4, "\": {");
	}
}

char	*load_world(char *filename)
{
	FILE	*file;
	long	length;
	char	*buffer;

	file = fopen(filename, "rb");
	if (!file)
		exit_error("Error : can't open world.json");
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	fseek(file, 0, SEEK_SET);
	buffer = malloc(length + 1);
	if (!buffer)
		exit_error("Error : malloc failed for world_data");
	fread(buffer, 1, length, file);
	buffer[length] = '\0';
	fclose(file);
	return (buffer);
}
