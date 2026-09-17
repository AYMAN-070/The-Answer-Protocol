/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_look.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 16:15:20 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/17 16:44:26 by ayhammou         ###   ########.fr       */
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
			strcat(out, "\"");
			strcat(out, dir[i]);
			strcat(out, "\": \"");
			strcat(out, d);
			strcat(out, "\"");
			first = 0;
		}
	}
}

void	handle_look(t_player *p, t_player *c, char *wld)
{
	char	key[64];
	char	name[64];
	char	p_list[256];
	char	e_list[256];
	char	out[1024];

	name[0] = '\0';
	sprintf(key, "\"%s\": {\n        \"name\":", p->location);
	extract_json_value(wld, key, name);
	if (name[0] == '\0')
		return ((void)write(p->sd, "ERR 404 ROOM_NOT_FOUND\n", 23));
	build_players_list(p, c, p_list);
	build_exits_list(wld, p->location, e_list);
	sprintf(out, "OK {\"room\": {\"id\": \"%s\", \"name\": \"%s\", "
		"\"description\": \"Salle\", \"exits\": {%s}}, "
		"\"players\": [%s], \"items\": [], \"npcs\": []}\n",
		p->location, name, e_list, p_list);
	write(p->sd, out, strlen(out));
}
