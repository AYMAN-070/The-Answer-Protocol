/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:18:03 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/10 14:37:45 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "map.h"
#include "layout.h"
#include <string.h>

static void	draw_node(int pos_x, int pos_y, t_room_state *room_state,
				const char *id)
{
	int		is_current;
	Color	node_color;
	Color	border_color;

	is_current = (room_state && room_state->name[0]
			&& !strcmp(room_state->name, id));
	if (is_current)
	{
		node_color = GREEN;
		border_color = LIGHTGRAY;
	}
	else
	{
		node_color = DARKGRAY;
		border_color = GRAY;
	}
	DrawRectangle(pos_x - 10, pos_y - 5, 20, 10, node_color);
	DrawRectangleLines(pos_x - 10, pos_y - 5, 20, 10, border_color);
}

static void	draw_bg(int pos_x, int pos_y, int width, int height)
{
	DrawRectangle(pos_x, pos_y, width, height, Fade(BLACK, 0.8f));
	DrawRectangleLines(pos_x, pos_y, width, height, Fade(LIGHTGRAY, 0.5f));
	DrawText("MINIMAP", pos_x + 5, pos_y + 5, 10, Fade(WHITE, 0.6f));
}

static void	draw_minimap_connections(int center_x, int center_y, int spacing)
{
	DrawLine(center_x, center_y, center_x, center_y + spacing, GRAY);
	DrawLine(center_x, center_y, center_x, center_y - 55, GRAY);
	DrawLine(center_x, center_y, center_x - (spacing * 2), center_y, GRAY);
	DrawLine(center_x, center_y, center_x + spacing, center_y, GRAY);
	DrawLine(center_x + spacing, center_y, center_x + spacing,
		center_y - 55, GRAY);
	DrawLine(center_x + spacing, center_y - 55, center_x, center_y - 55,
		GRAY);
}

static void	draw_minimap_nodes(int center_x, int center_y, int spacing,
				t_room_state *room_state)
{
	draw_node(center_x, center_y, room_state, "Main Corridor");
	draw_node(center_x, center_y + spacing, room_state, "Airlock");
	draw_node(center_x, center_y - spacing, room_state, "Armory");
	draw_node(center_x, center_y - 55, room_state, "Medbay");
	draw_node(center_x - spacing, center_y, room_state, "Maintenance Bay");
	draw_node(center_x - (spacing * 2), center_y, room_state, "AI Core");
	draw_node(center_x + spacing, center_y, room_state, "Crew Quarters");
	draw_node(center_x + spacing, center_y - 55, room_state, "Cafeteria");
}

void	render_minimap(int bottom_left_x, int bottom_left_y,
			t_room_state *room_state)
{
	int	spacing;
	int	start_y;
	int	center_x;
	int	center_y;

	spacing = 35;
	start_y = bottom_left_y - (spacing * 3 + 20);
	draw_bg(bottom_left_x, start_y, spacing * 5, spacing * 3 + 20);
	center_x = bottom_left_x + ((spacing * 5) / 2) + 17;
	center_y = start_y + ((spacing * 3 + 20) / 2) + 10;
	draw_minimap_connections(center_x, center_y, spacing);
	draw_minimap_nodes(center_x, center_y, spacing, room_state);
}
