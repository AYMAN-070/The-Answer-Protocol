/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_look.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 16:15:20 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/19 00:01:03 by ayhammou         ###   ########.fr       */
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
	char	n[64];
	char	*p;
	char	*l;

	out[0] = '\0';
	manage_dropped(3, room, NULL, out);
	sprintf(n, "\"%s\": {", room);
	p = strstr(wld, n);
	if (p)
		p = strstr(p, "\"items\": [");
	l = p ? strchr(p, ']') : NULL;
	p = p ? strchr(p, '[') : NULL;
	while (p && l)
	{
		p = strchr(p + 1, '"');
		if (!p || p > l)
			break ;
		sprintf(n, "%.*s", (int)(strchr(p + 1, '"') - (p + 1)), p + 1);
		p = strchr(p + 1, '"');
		if (!manage_taken(1, n) && out[0] != '\0')
			strcat(out, ", ");
		if (!manage_taken(1, n))
			sprintf(out + strlen(out), "\"%s\"", n);
	}
}

static void	build_npcs_list(char *wld, char *room, char *out)
{
	char	n[64];
	char	*p;
	char	*l;

	out[0] = '\0';
	sprintf(n, "\"%s\": {", room);
	p = strstr(wld, n);
	if (p)
		p = strstr(p, "\"spawns\": [");
	if (!p)
		return ;
	l = strchr(p, ']');
	p = strstr(p, "\"npc_type\": \"");
	while (l && p && p < l)
	{
		p += 13;
		sprintf(n, "%.*s", (int)(strchr(p, '"') - p), p);
		if (apply_damage_to_npc(room, n, 0, wld) > 0)
		{
			if (out[0] != '\0')
				strcat(out, ", ");
			sprintf(out + strlen(out), "\"%s\"", n);
		}
		p = strstr(p, "\"npc_type\": \"");
	}
}

void	handle_look(t_player *p, t_player *c, char *wld)
{
	char	b[6][256];
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
	get_room_desc(wld, p->location, b[5]);
	snprintf(out, sizeof(out), "OK {\"room\": {\"id\": \"%s\", "
		"\"name\": \"%s\", \"description\": \"%s\", "
		"\"exits\": {%s}}, \"players\": [%s], "
		"\"items\": [%s], \"npcs\": [%s]}\n",
		p->location, b[0], b[5], b[2], b[1], b[3], b[4]);
	printf("DEBUG LOOK JSON: %s", out);
	write(p->sd, out, strlen(out));
}
