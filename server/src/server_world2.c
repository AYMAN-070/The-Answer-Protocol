/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_world2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhammou <ayhammou@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 10:08:35 by ayhammou          #+#    #+#             */
/*   Updated: 2026/09/23 10:26:56 by ayhammou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include "cJSON.h"

char	*load_world(char *filename)
{
	FILE	*f;
	long	len;
	char	*buf;

	f = fopen(filename, "rb");
	if (!f)
		exit_error("ERR: Cannot open world.json (missing file)");
	fseek(f, 0, SEEK_END);
	len = ftell(f);
	if (len <= 0)
		exit_error("ERR: world.json is empty or invalid");
	fseek(f, 0, SEEK_SET);
	buf = malloc(len + 1);
	if (!buf || fread(buf, 1, len, f) != (size_t)len)
		exit_error("ERR: Failed to read world.json");
	buf[len] = '\0';
	fclose(f);
	return (buf);
}

static void	validate_locations(cJSON *locations, cJSON *json, char *wld)
{
	cJSON	*room;
	char	*room_name;

	room = locations->child;
	while (room)
	{
		room_name = room->string;
		if (!room_name || strlen(room_name) >= 64
			|| !check_room_exits(wld, room_name))
		{
			cJSON_Delete(json);
			exit_error("ERR: Invalid room or exits in world.json");
		}
		room = room->next;
	}
}

void	validate_world(char *wld)
{
	cJSON	*json;
	cJSON	*world;
	cJSON	*locations;
	cJSON	*items;

	json = cJSON_Parse(wld);
	if (!json)
		exit_error("ERR: Invalid JSON syntax in world.json");
	world = cJSON_GetObjectItemCaseSensitive(json, "world");
	if (!cJSON_IsObject(world))
	{
		cJSON_Delete(json);
		exit_error("ERR: Missing 'world' object at root");
	}
	locations = cJSON_GetObjectItemCaseSensitive(world, "locations");
	items = cJSON_GetObjectItemCaseSensitive(world, "items");
	if (!cJSON_IsObject(locations) || !cJSON_IsObject(items))
	{
		cJSON_Delete(json);
		exit_error("ERR: Missing 'locations' or 'items' in world");
	}
	validate_locations(locations, json, wld);
	cJSON_Delete(json);
}
