/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:21:58 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/01 13:36:47 by qdecross         ###   ########.fr       */
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

void	init_graphics_assets(t_room_assets *assets)
{
	assets->count = 0;
	preload_room(assets, "Airlock");
	preload_room(assets, "Main Corridor");
	preload_room(assets, "Crew Quarters");
	preload_room(assets, "Cafeteria");
	preload_room(assets, "Medbay");
	preload_room(assets, "Armory");
	preload_room(assets, "Maintenance Bay");
	preload_room(assets, "AI Core");
}

void	unload_graphics_assets(t_room_assets *assets)
{
	int	i;

	i = 0;
	while (i < assets->count)
	{
		UnloadTexture(assets->rooms[i].texture);
		i++;
	}
	assets->count = 0;
}

Texture2D	get_room_texture(const t_room_assets *assets,
		const char *room_name)
{
	int			i;
	Texture2D	empty;

	i = 0;
	while (i < assets->count)
	{
		if (strcmp(assets->rooms[i].name, room_name) == 0)
			return (assets->rooms[i].texture);
		i++;
	}
	memset(&empty, 0, sizeof(empty));
	return (empty);
}
