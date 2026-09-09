/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layout.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:27:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 13:28:58 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "layout.h"
#include "room_view.h"
#include "action_panel.h"
#include "status_panel.h"

#define HUD_PADDING 10

static void	draw_ui_panel(Rectangle bounds, const char *title)
{
	int	x;
	int	y;

	x = (int)bounds.x;
	y = (int)bounds.y;
	DrawRectangle(x, y, (int)bounds.width, (int)bounds.height, DARKGRAY);
	DrawRectangleLines(x, y, (int)bounds.width, (int)bounds.height, LIGHTGRAY);
	DrawText(title, x + 10, y + 10, 20, WHITE);
}

static t_hud_layout	compute_hud_layout(int sw, int sh)
{
	t_hud_layout	layout;
	int				bottom_y;
	int				bottom_h;

	layout.room.x = HUD_PADDING;
	layout.room.y = HUD_PADDING;
	layout.room.width = (sw * 65) / 100;
	layout.room.height = (sh * 70) / 100;
	layout.inventory.x = layout.room.width + (HUD_PADDING * 2);
	layout.inventory.y = HUD_PADDING;
	layout.inventory.width = sw - layout.room.width - (HUD_PADDING * 3);
	layout.inventory.height = layout.room.height;
	bottom_y = layout.room.height + (HUD_PADDING * 2);
	bottom_h = sh - layout.room.height - (HUD_PADDING * 3);
	layout.chat.x = HUD_PADDING;
	layout.chat.y = bottom_y;
	layout.chat.width = layout.room.width;
	layout.chat.height = bottom_h;
	layout.logs.x = layout.inventory.x;
	layout.logs.y = bottom_y;
	layout.logs.width = layout.inventory.width;
	layout.logs.height = bottom_h;
	return (layout);
}

void	render_main_layout(const t_game_assets *assets, t_ui_context *ctx)
{
	t_hud_layout	layout;

	layout = compute_hud_layout(GetScreenWidth(), GetScreenHeight());
	draw_ui_panel(layout.room, "Room View");
	draw_ui_panel(layout.inventory, "Inventory & Status");
	draw_ui_panel(layout.chat, "Chat");
	draw_ui_panel(layout.logs, "Action Logs");
	draw_room_view(assets, ctx, layout.room);
	render_status_panel(layout.inventory, &ctx->player, assets);
	render_action_panel(layout.inventory, ctx);
	render_message_panel(layout.chat, &ctx->chat, WHITE);
	render_message_panel(layout.logs, &ctx->logs, LIGHTGRAY);
}
