/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_view.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:27:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 09:57:16 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "room_view.h"
#include "map.h"
#include "graphics.h"
#include <stdio.h>

static void	draw_room_background(const t_game_assets *assets,
		t_room_state *state, Rectangle room_view)
{
	Texture2D	bg;
	Rectangle	source;
	Vector2		origin;

	if (!state || state->name[0] == '\0')
	{
		DrawRectangleRec(room_view, BLACK);
		return ;
	}
	bg = get_room_texture(assets, state->name);
	if (bg.id == 0)
	{
		DrawRectangleRec(room_view, BLACK);
		return ;
	}
	source.x = 0.0f;
	source.y = 0.0f;
	source.width = (float)bg.width;
	source.height = (float)bg.height;
	origin.x = 0.0f;
	origin.y = 0.0f;
	DrawTexturePro(bg, source, room_view, origin, 0.0f, WHITE);
}

static void	draw_room_description_overlay(t_room_state *st, Rectangle rect)
{
	Rectangle	bounds;
	const char	*title;
	const char	*desc;

	title = "Awaiting Connection";
	desc = "Synchronizing telemetry with Prometheus Station...";
	bounds.height = 120;
	bounds.width = rect.width - 100;
	bounds.x = rect.x + 50;
	bounds.y = rect.y + rect.height - 150;
	if (st && st->current_dialogue[0] != '\0')
	{
		title = st->current_npc;
		desc = st->current_dialogue;
	}
	else if (st && st->name[0] != '\0')
	{
		title = st->name;
		desc = st->description;
	}
	DrawRectangleRec(bounds, Fade(BLACK, 0.75f));
	DrawRectangleLinesEx(bounds, 2.0f, Fade(LIGHTGRAY, 0.5f));
	DrawText(title, (int)bounds.x + 15, (int)bounds.y + 15, 24, WHITE);
	DrawText(desc, (int)bounds.x + 15, (int)bounds.y + 50, 18, LIGHTGRAY);
}

static void	draw_minimap_overlay(Rectangle room_panel)
{
	int	map_anchor_x;
	int	map_anchor_y;

	map_anchor_x = (int)(room_panel.x + room_panel.width) - 175 - 15;
	map_anchor_y = (int)room_panel.y + 30 + 15 + 125;
	render_minimap(map_anchor_x, map_anchor_y);
}

static void	draw_player_counters(Rectangle room_panel, t_room_state *state)
{
	int		bx;
	int		by;
	char	txt_rm[64];
	char	txt_sv[64];

	bx = (int)room_panel.x + 15;
	by = (int)room_panel.y + 45;
	snprintf(txt_rm, 64, "Room Players: %d", state->room_players);
	snprintf(txt_sv, 64, "Server Total: %d", state->server_players);
	DrawRectangle(bx - 5, by - 5, 180, 50, Fade(BLACK, 0.75f));
	DrawRectangleLines(bx - 5, by - 5, 180, 50, Fade(LIGHTGRAY, 0.5f));
	DrawText(txt_rm, bx, by, 16, SKYBLUE);
	DrawText(txt_sv, bx, by + 22, 16, SKYBLUE);
}

void	draw_room_view(const t_game_assets *assets, t_room_state *state,
			Rectangle room_panel)
{
	Rectangle	view;

	view.x = room_panel.x + 2;
	view.y = room_panel.y + 30;
	view.width = room_panel.width - 4;
	view.height = room_panel.height - 32;
	draw_room_background(assets, state, view);
	draw_room_description_overlay(state, view);
	draw_minimap_overlay(room_panel);
	draw_player_counters(room_panel, state);
}
