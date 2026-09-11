/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_items.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 09:50:34 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 09:51:55 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"
#include <string.h>
#include <stdio.h>

static void	preload_item(t_item_assets *assets, const char *item_id)
{
	char		filepath[256];
	Texture2D	tex;

	if (assets->count >= MAX_ITEMS)
		return ;
	snprintf(filepath, sizeof(filepath),
		"assets/pictures/items/%s.png", item_id);
	tex = LoadTexture(filepath);
	strncpy(assets->items[assets->count].id, item_id, 31);
	assets->items[assets->count].texture = tex;
	assets->count++;
}

void	init_item_assets(t_item_assets *items)
{
	items->count = 0;
	preload_item(items, "red_keycard");
	preload_item(items, "laser_pistol");
	preload_item(items, "energy_cell");
}

void	unload_item_assets(t_item_assets *items)
{
	int	idx;

	idx = 0;
	while (idx < items->count)
	{
		UnloadTexture(items->items[idx].texture);
		idx++;
	}
	items->count = 0;
}

Texture2D	get_item_texture(const t_game_assets *assets, const char *id)
{
	int			idx;
	Texture2D	empty;

	idx = 0;
	while (idx < assets->items.count)
	{
		if (strcmp(assets->items.items[idx].id, id) == 0)
			return (assets->items.items[idx].texture);
		idx++;
	}
	memset(&empty, 0, sizeof(empty));
	return (empty);
}
