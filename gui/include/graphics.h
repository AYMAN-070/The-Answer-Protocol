/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:21:38 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/03 10:05:41 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPHICS_H
# define GRAPHICS_H

# include "raylib.h"

# define MAX_ROOMS 8
# define MAX_ITEMS 16

typedef struct s_room_asset
{
	char		name[64];
	Texture2D	texture;
}	t_room_asset;

typedef struct s_room_assets
{
	t_room_asset	rooms[MAX_ROOMS];
	int				count;
}	t_room_assets;

typedef struct s_item_asset
{
	char		id[32];
	Texture2D	texture;
}	t_item_asset;

typedef struct s_item_assets
{
	t_item_asset	items[MAX_ITEMS];
	int				count;
}	t_item_assets;

typedef struct s_game_assets
{
	t_room_assets	rooms;
	t_item_assets	items;
}	t_game_assets;

void		init_graphics_assets(t_game_assets *assets);
void		unload_graphics_assets(t_game_assets *assets);
Texture2D	get_room_texture(const t_game_assets *assets,
				const char *room_name);
Texture2D	get_item_texture(const t_game_assets *assets,
				const char *item_id);
void		init_item_assets(t_item_assets *items);
void		unload_item_assets(t_item_assets *items);

#endif
