/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_cmd_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 11:40:23 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/17 15:00:59 by ayhammou         ###   ########.fr       */
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

void	handle_look(t_player *p, t_player *c, char *wld)
{
	char	key[64];
	char	name[64];
	char	p_list[256];
	char	out[1024];

	name[0] = '\0';
	sprintf(key, "\"%s\": {\n        \"name\":", p->location);
	extract_json_value(wld, key, name);
	if (name[0] == '\0')
		return ((void)write(p->sd, "ERR 404 ROOM_NOT_FOUND\n", 23));
	build_players_list(p, c, p_list);
	sprintf(out, "OK {\"room\": {\"id\": \"%s\", \"name\": \"%s\"}, "
		"\"players\": [%s], \"items\": [], \"npcs\": []}\n",
		p->location, name, p_list);
	write(p->sd, out, strlen(out));
}

int	has_item(t_player *player, char *item)
{
	int	i;

	i = 0;
	while (i < 5)
	{
		if (strcmp(player->inventory[i], item) == 0)
			return (1);
		i++;
	}
	return (0);
}

static void	broadcast_room(t_player *c, char *room, char *msg, int exclude_sd)
{
	int	i;

	i = -1;
	while (++i < 10)
	{
		if (c[i].is_active && c[i].sd != exclude_sd 
			&& strcmp(c[i].location, room) == 0)
			write(c[i].sd, msg, strlen(msg));
	}
}

void	handle_move(t_player *p, t_player *c, char *buf, char *wld)
{
	char	arg[32];
	char	dest[32];
	char	out[128];

	arg[0] = '\0';
	dest[0] = '\0';
	if (sscanf(buf, "%*s %31s", arg) != 1 || arg[0] == '\0')
		return ((void)write(p->sd, "ERR missing direction\n", 22));
	get_room_exit(wld, p->location, arg, dest);
	if (dest[0] == '\0')
		return ((void)write(p->sd, "ERR invalid direction\n", 22));
	if (strcmp(dest, "ai_core") == 0 && has_item(p, "red_keycard") == 0)
		return ((void)write(p->sd,
				"ERR access denied : red_keycard required\n", 41));
	sprintf(out, "EVT ROOM PRESENCE LEAVE %s\n", p->name);
	broadcast_room(c, p->location, out, p->sd);
	strncpy(p->last_location, p->location, 31);
	strncpy(p->location, dest, 31);
	p->location[31] = '\0';
	sprintf(out, "EVT ROOM PRESENCE ENTER %s\n", p->name);
	broadcast_room(c, p->location, out, p->sd);

	handle_look(p, c, wld);
}
