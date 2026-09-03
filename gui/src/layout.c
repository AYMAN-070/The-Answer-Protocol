/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layout.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:27:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/03 08:36:08 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "layout.h"
#include "room_view.h"
#include "action_panel.h"
#include "state_parser.h"
#include <string.h>

#define HUD_PADDING 10

static void	draw_ui_panel(Rectangle bounds, const char *title)
{
	int	x;
	int	y;

	x = (int)bounds.x;
	y = (int)bounds.y;
	DrawRectangle(x, y, (int)bounds.width, (int)bounds.height, DARKGRAY);
	DrawRectangleLines(x, y, (int)bounds.width, (int)bounds.height,
		LIGHTGRAY);
	DrawText(title, x + 10, y + 10, 20, WHITE);
}

static t_hud_layout	compute_hud_layout(int sw, int sh)
{
	t_hud_layout	layout;

	layout.room.x = HUD_PADDING;
	layout.room.y = HUD_PADDING;
	layout.room.width = (sw * 65) / 100;
	layout.room.height = (sh * 70) / 100;
	layout.inventory.x = layout.room.width + (HUD_PADDING * 2);
	layout.inventory.y = HUD_PADDING;
	layout.inventory.width = sw - layout.room.width - (HUD_PADDING * 3);
	layout.inventory.height = layout.room.height;
	layout.chat.x = HUD_PADDING;
	layout.chat.y = layout.room.height + (HUD_PADDING * 2);
	layout.chat.width = sw - (HUD_PADDING * 2);
	layout.chat.height = sh - layout.room.height - (HUD_PADDING * 3);
	return (layout);
}

void	render_main_layout(const t_room_assets *assets, t_room_state *state,
			t_action_list *actions)
{
	t_hud_layout	layout;

	(void)state;
	layout = compute_hud_layout(GetScreenWidth(), GetScreenHeight());
	draw_ui_panel(layout.room, "Room View");
	draw_ui_panel(layout.inventory, "Inventory & Status");
	draw_ui_panel(layout.chat, "Chat & Action Logs");
	draw_room_view(assets, layout.room);
	render_action_panel(layout.inventory, actions);
}
