/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 09:03:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/11 14:54:46 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "layout.h"
#include "graphics.h"
#include <string.h>

static Vector3	get_layout(const char *id)
{
	const t_npc_layout	table[5] = {
	{"sec_drone", 0.6f, 0.6f, 0.65f},
	{"alpha_drone", 0.7f, 0.5f, 0.55f},
	{"injured_engineer", 1.0f, 0.65f, 0.70f},
	{"ai_terminal", 1.0f, 0.5f, 0.4f},
	{"cleaning_bot", 0.3f, 0.65f, 0.65f}};
	Vector3				layout;
	int					idx;

	layout.x = 0.45f;
	layout.y = 0.5f;
	layout.z = 0.5f;
	idx = -1;
	while (++idx < 5)
	{
		if (!strcmp(id, table[idx].id))
		{
			layout.x = table[idx].scale;
			layout.y = table[idx].pos_x_ratio;
			layout.z = table[idx].pos_y_ratio;
			return (layout);
		}
	}
	return (layout);
}

static Rectangle	get_npc_dest(Rectangle view_rect, Texture2D tex,
				const char *id)
{
	Vector3		layout;
	Rectangle	dest;
	float		scale;

	layout = get_layout(id);
	scale = ((view_rect.height * 0.5f) / (float)tex.height) * layout.x;
	dest.width = (float)tex.width * scale;
	dest.height = (float)tex.height * scale;
	dest.x = view_rect.x + (view_rect.width * layout.y) - (dest.width / 2);
	dest.y = view_rect.y + (view_rect.height * layout.z) - (dest.height / 2);
	return (dest);
}

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
	slot.y = view_rect.y + view_rect.height - 96.0f;
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
		if (tex.id != 0)
		{
			dest = get_npc_dest(view_rect, tex, ctx->state.npcs[idx].id);
			DrawTexturePro(tex, (Rectangle){0.0f, 0.0f, (float)tex.width,
				(float)tex.height}, dest, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
			if (idx == ctx->sel_npc)
				DrawRectangleLinesEx(dest, 2.0f, YELLOW);
			if (CheckCollisionPointRec(GetMousePosition(), dest)
				&& IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				ctx->sel_npc = idx;
		}
	}
}
