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

static void	draw_inventory_item(Rectangle bounds, t_entity *item,
				const t_game_assets *assets, int y_offset)
{
	Texture2D	tex;
	Rectangle	src;
	Rectangle	dest;
	Vector2		origin;

	tex = get_item_texture(assets, item->id);
	if (tex.id != 0)
	{
		src.x = 0.0f;
		src.y = 0.0f;
		src.width = (float)tex.width;
		src.height = (float)tex.height;
		dest.x = bounds.x + 15;
		dest.y = (float)y_offset;
		dest.width = 30.0f;
		dest.height = 30.0f;
		origin.x = 0.0f;
		origin.y = 0.0f;
		DrawTexturePro(tex, src, dest, origin, 0.0f, WHITE);
	}
	DrawText(item->name, (int)bounds.x + 55, y_offset + 5, 20, WHITE);
}

static void	draw_inventory_list(Rectangle bounds, t_player_state *player,
				const t_game_assets *assets)
{
	int	idx;
	int	y_offset;

	DrawText("Inventory:", bounds.x, bounds.y, 20, LIGHTGRAY);
	if (player->inv_count == 0)
	{
		DrawText("Empty", bounds.x + 15, bounds.y + 30, 20, DARKGRAY);
		return ;
	}
	idx = 0;
	while (idx < player->inv_count)
	{
		y_offset = bounds.y + 30 + (idx * 35);
		draw_inventory_item(bounds, &player->inventory[idx], assets,
			y_offset);
		idx++;
	}
}

void	render_status_panel(Rectangle bounds, t_player_state *player,
			const t_game_assets *assets)
{
	Rectangle	hp_bounds;
	Rectangle	inv_bounds;

	hp_bounds.x = bounds.x + 15;
	hp_bounds.y = bounds.y + 40;
	hp_bounds.width = bounds.width - 30;
	hp_bounds.height = 25;
	draw_hp_bar(hp_bounds, player);
	inv_bounds.x = bounds.x + 15;
	inv_bounds.y = hp_bounds.y + 40;
	inv_bounds.width = bounds.width - 30;
	inv_bounds.height = (bounds.height / 2.0f) - 95.0f;
	draw_inventory_list(inv_bounds, player, assets);
}
