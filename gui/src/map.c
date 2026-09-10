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

static void	draw_node(int x, int y, t_room_state *st, const char *id)
{
	int		is_cur;
	Color	n_col;
	Color	b_col;

	is_cur = (st && st->name[0] && !strcmp(st->name, id));
	if (is_cur)
	{
		n_col = GREEN;
		b_col = LIGHTGRAY;
	}
	else
	{
		n_col = DARKGRAY;
		b_col = GRAY;
	}
	DrawRectangle(x - 10, y - 5, 20, 10, n_col);
	DrawRectangleLines(x - 10, y - 5, 20, 10, b_col);
}

static void	draw_bg(int x, int y, int width, int height)
{
	DrawRectangle(x, y, width, height, Fade(BLACK, 0.8f));
	DrawRectangleLines(x, y, width, height, Fade(LIGHTGRAY, 0.5f));
	DrawText("MINIMAP", x + 5, y + 5, 10, Fade(WHITE, 0.6f));
}

static void	draw_minimap_connections(int cx, int cy, int spc)
{
	DrawLine(cx, cy, cx, cy + spc, GRAY);
	DrawLine(cx, cy, cx, cy - 55, GRAY);
	DrawLine(cx, cy, cx - (spc * 2), cy, GRAY);
	DrawLine(cx, cy, cx + spc, cy, GRAY);
	DrawLine(cx + spc, cy, cx + spc, cy - 55, GRAY);
	DrawLine(cx + spc, cy - 55, cx, cy - 55, GRAY);
}

static void	draw_minimap_nodes(int cx, int cy, int spc, t_room_state *st)
{
	draw_node(cx, cy, st, "Main Corridor");
	draw_node(cx, cy + spc, st, "Airlock");
	draw_node(cx, cy - spc, st, "Armory");
	draw_node(cx, cy - 55, st, "Medbay");
	draw_node(cx - spc, cy, st, "Maintenance Bay");
	draw_node(cx - (spc * 2), cy, st, "AI Core");
	draw_node(cx + spc, cy, st, "Crew Quarters");
	draw_node(cx + spc, cy - 55, st, "Cafeteria");
}

void	render_minimap(int bl_x, int bl_y, t_room_state *st)
{
	int	spc;
	int	start_y;
	int	cx;
	int	cy;

	spc = 35;
	start_y = bl_y - (spc * 3 + 20);
	draw_bg(bl_x, start_y, spc * 5, spc * 3 + 20);
	cx = bl_x + ((spc * 5) / 2) + 17;
	cy = start_y + ((spc * 3 + 20) / 2) + 10;
	draw_minimap_connections(cx, cy, spc);
	draw_minimap_nodes(cx, cy, spc, st);
}
