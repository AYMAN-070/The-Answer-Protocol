/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layout.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:27:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/11 09:59:24 by qdecross         ###   ########.fr       */
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
	t_hud_layout	l;

	l.room.x = 10;
	l.room.y = 10;
	l.room.width = (sw * 65) / 100;
	l.room.height = (sh * 70) / 100;
	l.inventory.x = l.room.width + 20;
	l.inventory.y = 10;
	l.inventory.width = sw - l.room.width - 30;
	l.inventory.height = l.room.height;
	l.chat.x = 10;
	l.chat.y = l.room.height + 20;
	l.chat.width = l.room.width;
	l.chat.height = sh - l.room.height - 30;
	l.logs.x = l.inventory.x;
	l.logs.y = l.chat.y;
	l.logs.width = l.inventory.width;
	l.logs.height = l.chat.height;
	return (l);
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
