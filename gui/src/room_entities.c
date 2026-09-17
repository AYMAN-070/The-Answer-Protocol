/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 09:03:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/17 10:32:37 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "layout.h"
#include "graphics.h"
#include "room_view.h"
#include <string.h>

static void	draw_item_slot(Rectangle slot, Texture2D tex, const char *label,
				int selected)
{
	Color	border;

	border = LIGHTGRAY;
	if (selected)
		border = YELLOW;
	DrawRectangleRec(slot, Fade(BLACK, 0.55f));
	if (tex.id != 0)
		DrawTexturePro(tex, (Rectangle){0.0f, 0.0f, (float)tex.width,
			(float)tex.height}, slot, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
	DrawRectangleLinesEx(slot, 2.0f, border);
	DrawText(label, (int)slot.x, (int)(slot.y + slot.height + 4), 10, border);
}

void	draw_room_items(const t_game_assets *assets, t_ui_context *ctx,
			Rectangle view_rect)
{
	Rectangle	slot;
	int			idx;

	if (ctx->sel_room_item >= ctx->state.item_count)
		ctx->sel_room_item = 0;
	slot.width = 56.0f;
	slot.height = 56.0f;
	slot.y = view_rect.y + 95.0f;
	idx = -1;
	while (++idx < ctx->state.item_count)
	{
		slot.x = view_rect.x + 20.0f + (float)idx * 70.0f;
		if (CheckCollisionPointRec(GetMousePosition(), slot)
			&& IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			ctx->sel_room_item = idx;
		draw_item_slot(slot, get_item_texture(assets,
				ctx->state.items[idx].id), ctx->state.items[idx].name,
			idx == ctx->sel_room_item);
	}
}

static void	draw_npc_sprite(Rectangle dest, Texture2D tex, const char *name,
				int selected)
{
	if (tex.id != 0)
		DrawTexturePro(tex, (Rectangle){0.0f, 0.0f, (float)tex.width,
			(float)tex.height}, dest, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
	else
	{
		DrawRectangleRec(dest, Fade(DARKPURPLE, 0.85f));
		DrawText(name, (int)dest.x + 4, (int)dest.y + 4, 10, WHITE);
	}
	if (selected)
		DrawRectangleLinesEx(dest, 2.0f, YELLOW);
}

void	draw_room_npcs(const t_game_assets *assets, t_ui_context *ctx,
			Rectangle view_rect)
{
	int			idx;
	Texture2D	tex;
	Rectangle	dest;

	if (ctx->sel_npc >= ctx->state.npc_count)
		ctx->sel_npc = 0;
	idx = -1;
	while (++idx < ctx->state.npc_count)
	{
		tex = get_npc_texture(assets, ctx->state.npcs[idx].id);
		dest = get_npc_dest(view_rect, tex, ctx->state.npcs[idx].id);
		draw_npc_sprite(dest, tex, ctx->state.npcs[idx].name,
			idx == ctx->sel_npc);
		if (CheckCollisionPointRec(GetMousePosition(), dest)
			&& IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			ctx->sel_npc = idx;
	}
}
