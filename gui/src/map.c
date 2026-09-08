/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:18:03 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/01 10:30:07 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "map.h"

static void	draw_minimap_node(int x, int y, int is_current)
{
	Color	node_color;
	Color	border_color;

	if (is_current)
		node_color = GREEN;
	else
		node_color = DARKGRAY;
	if (is_current)
		border_color = LIGHTGRAY;
	else
		border_color = GRAY;
	DrawRectangle(x - 10, y - 5, 20, 10, node_color);
	DrawRectangleLines(x - 10, y - 5, 20, 10, border_color);
}

static void	draw_minimap_background(int x, int y, int width, int height)
{
	DrawRectangle(x, y, width, height, Fade(BLACK, 0.8f));
	DrawRectangleLines(x, y, width, height, Fade(LIGHTGRAY, 0.5f));
	DrawText("MINIMAP", x + 5, y + 5, 10, Fade(WHITE, 0.6f));
}

static void	draw_minimap_connections(int cx, int cy, int spacing)
{
	DrawLine(cx, cy, cx, cy + spacing, GRAY);
	DrawLine(cx, cy, cx + spacing, cy, GRAY);
	DrawLine(cx + spacing, cy, cx + spacing, cy - spacing, GRAY);
	DrawLine(cx + spacing, cy - spacing, cx, cy - spacing, GRAY);
	DrawLine(cx, cy - spacing, cx, cy, GRAY);
	DrawLine(cx, cy, cx - spacing, cy, GRAY);
	DrawLine(cx - spacing, cy, cx - (spacing * 2), cy, GRAY);
}

static void	draw_minimap_nodes(int cx, int cy, int spacing)
{
	draw_minimap_node(cx, cy + spacing, 0);
	draw_minimap_node(cx, cy, 1);
	draw_minimap_node(cx + spacing, cy, 0);
	draw_minimap_node(cx + spacing, cy - spacing, 0);
	draw_minimap_node(cx, cy - spacing, 0);
	draw_minimap_node(cx, cy - (spacing / 2), 0);
	draw_minimap_node(cx - spacing, cy, 0);
	draw_minimap_node(cx - (spacing * 2), cy, 0);
}

void	render_minimap(int bottom_left_x, int bottom_left_y)
{
	int	spacing;
	int	start_y;
	int	cx;
	int	cy;

	spacing = 35;
	start_y = bottom_left_y - (spacing * 3 + 20);
	draw_minimap_background(bottom_left_x, start_y,
		spacing * 5, spacing * 3 + 20);
	cx = bottom_left_x + ((spacing * 5) / 2) + 17;
	cy = start_y + ((spacing * 3 + 20) / 2) + 10;
	draw_minimap_connections(cx, cy, spacing);
	draw_minimap_nodes(cx, cy, spacing);
}
