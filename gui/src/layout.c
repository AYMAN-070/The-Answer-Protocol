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
	int	pos_x;
	int	pos_y;

	pos_x = (int)bounds.x;
	pos_y = (int)bounds.y;
	DrawRectangle(pos_x, pos_y, (int)bounds.width, (int)bounds.height,
		DARKGRAY);
	DrawRectangleLines(pos_x, pos_y, (int)bounds.width, (int)bounds.height,
		LIGHTGRAY);
	DrawText(title, pos_x + 10, pos_y + 10, 20, WHITE);
}

static t_hud_layout	compute_hud_layout(int screen_w, int screen_h)
{
	t_hud_layout	layout;

	layout.room.x = 10;
	layout.room.y = 10;
	layout.room.width = (screen_w * 65) / 100;
	layout.room.height = (screen_h * 70) / 100;
	layout.inventory.x = layout.room.width + 20;
	layout.inventory.y = 10;
	layout.inventory.width = screen_w - layout.room.width - 30;
	layout.inventory.height = layout.room.height;
	layout.chat.x = 10;
	layout.chat.y = layout.room.height + 20;
	layout.chat.width = layout.room.width;
	layout.chat.height = screen_h - layout.room.height - 30;
	layout.logs.x = layout.inventory.x;
	layout.logs.y = layout.chat.y;
	layout.logs.width = layout.inventory.width;
	layout.logs.height = layout.chat.height;
	return (layout);
}

void	render_main_layout(const t_game_assets *assets, t_ui_context *ctx)
{
	t_hud_layout	layout;

	layout = compute_hud_layout(GetScreenWidth(), GetScreenHeight());
	draw_ui_panel(layout.room, "Room View");
	draw_ui_panel(layout.inventory, "Inventory & Status");
	draw_ui_panel(layout.chat, "Chat (TAB: Room/Global)");
	draw_ui_panel(layout.logs, "Action Logs");
	draw_room_view(assets, ctx, layout.room);
	render_status_panel(layout.inventory, &ctx->player, assets);
	render_action_panel(layout.inventory, ctx);
	render_chat_panel(layout.chat, ctx);
	render_message_panel(layout.logs, &ctx->logs, LIGHTGRAY);
}
