/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_look.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 16:15:20 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/17 17:42:51 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static void	build_players_list(t_player *p, t_player *c, char *p_list)
{
	int	i;

	p_list[0] = '\0';
	i = -1;
	while (++i < 10)
	{
		if (c[i].is_active && strcmp(c[i].location, p->location) == 0)
		{
			if (p_list[0] != '\0')
				strcat(p_list, ", ");
			sprintf(p_list + strlen(p_list), "\"%s\"", c[i].name);
		}
	}
}

static void	build_exits_list(char *wld, char *room, char *out)
{
	char	d[64];
	char	*dir[4];
	int		i;
	int		first;

	dir[0] = "north";
	dir[1] = "south";
	dir[2] = "east";
	dir[3] = "west";
	out[0] = '\0';
	first = 1;
	i = -1;
	while (++i < 4)
	{
		get_room_exit(wld, room, dir[i], d);
		if (d[0] != '\0')
		{
			if (!first)
				strcat(out, ", ");
			sprintf(out + strlen(out), "\"%s\": \"%s\"", dir[i], d);
			first = 0;
		}
	}
}

static void	build_items_list(char *wld, char *room, char *out)
{
	char	key[64];
	char	*pos;
	char	*end;

	out[0] = '\0';
	sprintf(key, "\"%s\": {", room);
	pos = strstr(wld, key);
	if (!pos)
		return ;
	pos = strstr(pos, "\"items\": [");
	if (!pos)
		return ;
	pos += 10;
	end = strchr(pos, ']');
	if (end && (end - pos) < 255)
	{
		strncpy(out, pos, end - pos);
		out[end - pos] = '\0';
	}
}

static void	build_npcs_list(char *wld, char *room, char *out)
{
	char	key[64];
	char	*pos;
	char	*end;
	int		first;

	first = 1;
	out[0] = '\0';
	sprintf(key, "\"%s\": {", room);
	pos = strstr(wld, key);
	if (pos)
		pos = strstr(pos, "\"spawns\": [");
	if (pos)
		pos = strstr(pos, "\"npc_type\": \"");
	while (pos)
	{
		pos += 13;
		end = strchr(pos, '"');
		if (!first)
			strcat(out, ", ");
		strcat(out, "\"");
		strncat(out, pos, end - pos);
		strcat(out, "\"");
		first = 0;
		pos = strstr(pos, "\"npc_type\": \"");
	}
}

void	handle_look(t_player *p, t_player *c, char *wld)
{
	char	b[5][256];
	char	out[2048];

	sprintf(out, "\"%s\": {\n        \"name\":", p->location);
	b[0][0] = '\0';
	extract_json_value(wld, out, b[0]);
	if (b[0][0] == '\0')
		return ((void)write(p->sd, "ERR 404 ROOM_NOT_FOUND\n", 23));

	build_players_list(p, c, b[1]);
	build_exits_list(wld, p->location, b[2]);
	build_items_list(wld, p->location, b[3]);
	build_npcs_list(wld, p->location, b[4]);

	snprintf(out, sizeof(out), "OK {\"room\": {\"id\": \"%s\", \"name\": \"%s\", "
		"\"description\": \"Salle\", \"exits\": {%s}}, "
		"\"players\": [%s], \"items\": [%s], \"npcs\": [%s]}\n",
		p->location, b[0], b[2], b[1], b[3], b[4]);
	write(p->sd, out, strlen(out));
}
