/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_view.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:27:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/10 14:38:06 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "room_view.h"
#include "map.h"
#include "graphics.h"
#include "layout.h"
#include <stdio.h>
#include <string.h>

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

static void	apply_room_lighting(Rectangle b, t_ui_context *ctx)
{
	int				i;
	t_player_state	*p;

	p = &ctx->player;
	if (ctx->state.name[0] == '\0')
	{
		DrawText("No data. Use LOOK.", (int)b.x + ((int)b.width - 200) / 2,
			(int)b.y + ((int)b.height - 20) / 2, 20, GRAY);
		return ;
	}
	if (p->known_rooms[31][0] == 'P' && p->known_count < 31)
		strcpy(p->known_rooms[p->known_count++], ctx->state.name);
	p->known_rooms[31][0] = '\0';
	i = -1;
	while (++i < p->known_count)
		if (!strcmp(p->known_rooms[i], ctx->state.name))
			return ;
	DrawRectangleRec(b, Fade(BLACK, 0.65f));
	DrawText("Unexplored area. Use LOOK.",
		(int)b.x + ((int)b.width - 280) / 2,
		(int)b.y + ((int)b.height - 20) / 2, 20, LIGHTGRAY);
	ctx->state.description[0] = '\0';
}

void	draw_room_view(const t_game_assets *assets, t_ui_context *ctx,
			Rectangle room_panel)
{
	Rectangle	view;
	int			map_x;
	int			map_y;

	view.x = room_panel.x + 2;
	view.y = room_panel.y + 30;
	view.width = room_panel.width - 4;
	view.height = room_panel.height - 32;
	draw_room_background(assets, &ctx->state, view);
	apply_room_lighting(view, ctx);
	draw_room_description_overlay(&ctx->state, view);
	map_x = (int)(room_panel.x + room_panel.width) - 190;
	map_y = (int)room_panel.y + 170;
	render_minimap(map_x, map_y, &ctx->state);
	draw_player_counters(room_panel, &ctx->state);
}
