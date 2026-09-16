/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_panel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 09:40:09 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/11 13:27:10 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "status_panel.h"
#include "raylib.h"
#include <stdio.h>

static void	draw_hp_bar(Rectangle bounds, t_player_state *player)
{
	float	hp_ratio;
	int		bar_width;
	char	hp_text[32];
	Color	hp_col;

	if (player->max_hp <= 0)
		player->max_hp = 100;
	hp_ratio = (float)player->hp / (float)player->max_hp;
	bar_width = (int)(bounds.width * hp_ratio);
	hp_col = GREEN;
	if (hp_ratio < 0.3f)
		hp_col = RED;
	else if (hp_ratio < 0.6f)
		hp_col = ORANGE;
	DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, DARKGRAY);
	DrawRectangle(bounds.x, bounds.y, bar_width, bounds.height, hp_col);
	DrawRectangleLines(bounds.x, bounds.y, bounds.width, bounds.height,
		LIGHTGRAY);
	snprintf(hp_text, sizeof(hp_text), "HP: %d / %d", player->hp,
		player->max_hp);
	DrawText(hp_text, bounds.x + 10, bounds.y + 3, 20, WHITE);
}

static void	draw_inventory_item(Rectangle row, t_entity *item,
				const t_game_assets *assets, int selected)
{
	Texture2D	tex;
	Color		text_color;

	text_color = WHITE;
	if (selected)
	{
		text_color = YELLOW;
		DrawRectangleRec(row, Fade(LIGHTGRAY, 0.25f));
	}
	tex = get_item_texture(assets, item->id);
	if (tex.id != 0)
		DrawTexturePro(tex, (Rectangle){0.0f, 0.0f, (float)tex.width,
			(float)tex.height}, (Rectangle){row.x + 15, row.y, 30.0f, 30.0f},
			(Vector2){0.0f, 0.0f}, 0.0f, WHITE);
	DrawText(item->name, (int)row.x + 55, (int)row.y + 5, 20, text_color);
}

static void	draw_inventory_list(Rectangle bounds, t_ui_context *ctx,
				const t_game_assets *assets)
{
	Rectangle	row;
	int			idx;

	DrawText("Inventory:", bounds.x, bounds.y, 20, LIGHTGRAY);
	if (ctx->player.inv_count == 0)
	{
		DrawText("Empty", bounds.x + 15, bounds.y + 30, 20, DARKGRAY);
		return ;
	}
	row.x = bounds.x;
	row.width = bounds.width;
	row.height = 32.0f;
	idx = -1;
	while (++idx < ctx->player.inv_count)
	{
		row.y = bounds.y + 30.0f + (float)(idx * 35);
		if (CheckCollisionPointRec(GetMousePosition(), row)
			&& IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			ctx->sel_item = idx;
		draw_inventory_item(row, &ctx->player.inventory[idx], assets,
			idx == ctx->sel_item);
	}
}

void	render_status_panel(Rectangle bounds, t_ui_context *ctx,
			const t_game_assets *assets)
{
	Rectangle	hp_bounds;
	Rectangle	inv_bounds;

	if (ctx->sel_item >= ctx->player.inv_count)
		ctx->sel_item = 0;
	hp_bounds.x = bounds.x + 15;
	hp_bounds.y = bounds.y + 40;
	hp_bounds.width = bounds.width - 30;
	hp_bounds.height = 25;
	draw_hp_bar(hp_bounds, &ctx->player);
	inv_bounds.x = bounds.x + 15;
	inv_bounds.y = hp_bounds.y + 40;
	inv_bounds.width = bounds.width - 30;
	inv_bounds.height = (bounds.height / 2.0f) - 95.0f;
	draw_inventory_list(inv_bounds, ctx, assets);
}
