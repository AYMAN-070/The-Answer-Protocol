/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_view.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:27:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/11 09:49:34 by qdecross         ###   ########.fr       */
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
		t_room_state *state, Rectangle view)
{
	Texture2D	texture;
	Vector2		position;
	float		scale;

	DrawRectangleRec(view, BLACK);
	if (!state || state->name[0] == '\0')
		return ;
	texture = get_room_texture(assets, state->name);
	if (texture.id == 0)
		return ;
	scale = view.width / (float)texture.width;
	if (view.height / (float)texture.height < scale)
		scale = view.height / (float)texture.height;
	position.x = view.x + (view.width - ((float)texture.width * scale))
		/ 2.0f;
	position.y = view.y + (view.height - ((float)texture.height * scale))
		/ 2.0f;
	DrawTextureEx(texture, position, 0.0f, scale, WHITE);
}

static void	draw_room_description_overlay(t_room_state *state,
				Rectangle rect)
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
	if (state && state->current_dialogue[0] != '\0')
	{
		title = state->current_npc;
		desc = state->current_dialogue;
	}
	else if (state && state->name[0] != '\0')
	{
		title = state->name;
		desc = state->description;
	}
	DrawRectangleRec(bounds, Fade(BLACK, 0.75f));
	DrawRectangleLinesEx(bounds, 2.0f, Fade(LIGHTGRAY, 0.5f));
	DrawText(title, (int)bounds.x + 15, (int)bounds.y + 15, 24, WHITE);
	DrawText(desc, (int)bounds.x + 15, (int)bounds.y + 50, 18, LIGHTGRAY);
}

static void	draw_player_counters(Rectangle room_panel, t_room_state *state)
{
	int		box_x;
	int		box_y;
	char	room_text[64];
	char	server_text[64];

	box_x = (int)room_panel.x + 15;
	box_y = (int)room_panel.y + 45;
	snprintf(room_text, 64, "Room Players: %d", state->room_players);
	snprintf(server_text, 64, "Server Total: %d", state->server_players);
	DrawRectangle(box_x - 5, box_y - 5, 180, 50, Fade(BLACK, 0.75f));
	DrawRectangleLines(box_x - 5, box_y - 5, 180, 50, Fade(LIGHTGRAY, 0.5f));
	DrawText(room_text, box_x, box_y, 16, SKYBLUE);
	DrawText(server_text, box_x, box_y + 22, 16, SKYBLUE);
}

static void	apply_room_lighting(Rectangle bounds, t_ui_context *ctx)
{
	int				idx;
	t_player_state	*player;

	player = &ctx->player;
	if (ctx->state.name[0] == '\0')
	{
		DrawText("No data. Use LOOK.",
			(int)bounds.x + ((int)bounds.width - 200) / 2,
			(int)bounds.y + ((int)bounds.height - 20) / 2, 20, GRAY);
		return ;
	}
	if (player->known_rooms[31][0] == 'P' && player->known_count < 31)
		strcpy(player->known_rooms[player->known_count++], ctx->state.name);
	player->known_rooms[31][0] = '\0';
	idx = -1;
	while (++idx < player->known_count)
		if (!strcmp(player->known_rooms[idx], ctx->state.name))
			return ;
	DrawRectangleRec(bounds, Fade(BLACK, 0.65f));
	DrawText("Unexplored area. Use LOOK.",
		(int)bounds.x + ((int)bounds.width - 280) / 2,
		(int)bounds.y + ((int)bounds.height - 20) / 2, 20, LIGHTGRAY);
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
	draw_room_npcs(assets, ctx, view);
	apply_room_lighting(view, ctx);
	draw_room_description_overlay(&ctx->state, view);
	map_x = (int)(room_panel.x + room_panel.width) - 190;
	map_y = (int)room_panel.y + 170;
	render_minimap(map_x, map_y, &ctx->state);
	draw_player_counters(room_panel, &ctx->state);
}
