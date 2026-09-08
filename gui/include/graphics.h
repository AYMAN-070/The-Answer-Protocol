/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:21:38 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/01 13:26:04 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPHICS_H
# define GRAPHICS_H

# include "raylib.h"

# define MAX_ROOMS 8

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

void		init_graphics_assets(t_room_assets *assets);
void		unload_graphics_assets(t_room_assets *assets);
Texture2D	get_room_texture(const t_room_assets *assets,
				const char *room_name);

#endif
