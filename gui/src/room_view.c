/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_view.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:27:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/01 13:57:05 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "room_view.h"
#include "map.h"
#include "graphics.h"

static Rectangle	get_room_view_bounds(Rectangle room_panel)
{
	Rectangle	view;

	view.x = room_panel.x + 2;
	view.y = room_panel.y + 30;
	view.width = room_panel.width - 4;
	view.height = room_panel.height - 32;
	return (view);
}

static void	draw_room_background(const t_room_assets *assets,
		Rectangle room_view)
{
	Texture2D	bg;
	Rectangle	source;
	Vector2		origin;

	bg = get_room_texture(assets, "Main Corridor");
	if (bg.id == 0)
		return ;
	source.x = 0.0f;
	source.y = 0.0f;
	source.width = (float)bg.width;
	source.height = (float)bg.height;
	origin.x = 0.0f;
	origin.y = 0.0f;
	DrawTexturePro(bg, source, room_view, origin, 0.0f, WHITE);
}

static void	draw_room_description_overlay(const char *title,
		const char *description, Rectangle view_rect)
{
	int			box_width;
	int			box_height;
	int			box_x;
	int			box_y;
	Rectangle	bounds;

	box_height = 120;
	box_width = (int)view_rect.width - 100;
	box_x = (int)view_rect.x + 50;
	box_y = (int)view_rect.y + (int)view_rect.height - box_height - 30;
	bounds.x = (float)box_x;
	bounds.y = (float)box_y;
	bounds.width = (float)box_width;
	bounds.height = (float)box_height;
	DrawRectangleRec(bounds, Fade(BLACK, 0.75f));
	DrawRectangleLinesEx(bounds, 2.0f, Fade(LIGHTGRAY, 0.5f));
	DrawText(title, box_x + 15, box_y + 15, 24, WHITE);
	DrawText(description, box_x + 15, box_y + 50, 18, LIGHTGRAY);
}

static void	draw_minimap_overlay(Rectangle room_panel)
{
	int	map_anchor_x;
	int	map_anchor_y;

	map_anchor_x = (int)(room_panel.x + room_panel.width) - 175 - 15;
	map_anchor_y = (int)room_panel.y + 30 + 15 + 125;
	render_minimap(map_anchor_x, map_anchor_y);
}

void	draw_room_view(const t_room_assets *assets, Rectangle room_panel)
{
	Rectangle	view;

	view = get_room_view_bounds(room_panel);
	draw_room_background(assets, view);
	draw_room_description_overlay("Main Corridor",
		"The central artery of the station. Red emergency lights are flashing.",
		view);
	draw_minimap_overlay(room_panel);
}
