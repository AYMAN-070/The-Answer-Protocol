/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:21:58 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/11 09:13:30 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"
#include <string.h>
#include <stdio.h>

static void	preload_room(t_room_assets *assets, const char *room_name)
{
	char		filepath[256];
	Texture2D	tex;

	if (assets->count >= MAX_ROOMS)
		return ;
	snprintf(filepath, sizeof(filepath),
		"assets/pictures/rooms/%s.png", room_name);
	tex = LoadTexture(filepath);
	strncpy(assets->rooms[assets->count].name, room_name,
		sizeof(assets->rooms[assets->count].name) - 1);
	assets->rooms[assets->count].texture = tex;
	assets->count++;
}

void	init_graphics_assets(t_game_assets *assets)
{
	assets->rooms.count = 0;
	preload_room(&assets->rooms, "Airlock");
	preload_room(&assets->rooms, "Main Corridor");
	preload_room(&assets->rooms, "Crew Quarters");
	preload_room(&assets->rooms, "Cafeteria");
	preload_room(&assets->rooms, "Medbay");
	preload_room(&assets->rooms, "Armory");
	preload_room(&assets->rooms, "Maintenance Bay");
	preload_room(&assets->rooms, "AI Core");
	init_item_assets(&assets->items);
	init_npc_assets(&assets->npcs);
}

void	unload_graphics_assets(t_game_assets *assets)
{
	int	idx;

	idx = 0;
	while (idx < assets->rooms.count)
	{
		UnloadTexture(assets->rooms.rooms[idx].texture);
		idx++;
	}
	assets->rooms.count = 0;
	unload_item_assets(&assets->items);
	unload_npc_assets(&assets->npcs);
}

Texture2D	get_room_texture(const t_game_assets *assets,
		const char *room_name)
{
	int			idx;
	Texture2D	empty;

	idx = 0;
	while (idx < assets->rooms.count)
	{
		if (strcmp(assets->rooms.rooms[idx].name, room_name) == 0)
			return (assets->rooms.rooms[idx].texture);
		idx++;
	}
	memset(&empty, 0, sizeof(empty));
	return (empty);
}
