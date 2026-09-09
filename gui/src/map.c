/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:18:03 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 13:32:17 by qdecross         ###   ########.fr       */
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
	n_col = is_cur ? GREEN : DARKGRAY;
	b_col = is_cur ? LIGHTGRAY : GRAY;
	DrawRectangle(x - 10, y - 5, 20, 10, n_col);
	DrawRectangleLines(x - 10, y - 5, 20, 10, b_col);
}

static void	draw_bg(int x, int y, int width, int height)
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

static void	draw_minimap_nodes(int cx, int cy, int spc, t_room_state *st)
{
	draw_node(cx, cy + spc, st, "Airlock");
	draw_node(cx, cy, st, "Main Corridor");
	draw_node(cx + spc, cy, st, "Armory");
	draw_node(cx + spc, cy - spc, st, "Medbay");
	draw_node(cx, cy - spc, st, "AI Core");
	draw_node(cx, cy - (spc / 2), st, "Maintenance Bay");
	draw_node(cx - spc, cy, st, "Cafeteria");
	draw_node(cx - (spc * 2), cy, st, "Crew Quarters");
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
