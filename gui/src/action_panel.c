/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_panel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 11:18:36 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/11 14:16:27 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "action_panel.h"
#include "network.h"
#include "layout.h"
#include <string.h>
#include <stdio.h>

static void	handle_click(int id, t_ui_context *ctx)
{
	const char	*commands[18] = {"LOOK", "MOVE north", "MOVE west",
		"MOVE east", "MOVE south", "TAKE", "DROP", "TALK", "QUEST",
		"ATTACK", "DEFEND", "FLEE", "INVENTORY", "STATUS", "QUESTS",
		"WHO", "GROUP CREATE", "QUIT"};
	char		cmd_buf[64];

	snprintf(cmd_buf, 64, "%s\n", commands[id]);
	if ((id >= 7 && id <= 10) && ctx->state.npc_count > 0)
		snprintf(cmd_buf, 64, "%s %s\n", commands[id],
			ctx->state.npcs[ctx->sel_npc].id);
	else if (id == 5 && ctx->state.item_count > 0)
		snprintf(cmd_buf, 64, "%s %s\n", commands[id],
			ctx->state.items[ctx->sel_room_item].id);
	else if (id == 6 && ctx->player.inv_count > 0)
		snprintf(cmd_buf, 64, "%s %s\n", commands[id],
			ctx->player.inventory[ctx->sel_item].id);
	sscanf(cmd_buf, "%15s", ctx->last_cmd);
	net_send_command(ctx->sock_fd, cmd_buf);
	apply_npc_side_effects(ctx, id);
	if (id == 0)
		remember_room(ctx);
}

static void	update_btn_colors(int id, Rectangle btn_rect, t_ui_context *ctx,
				Color colors[2])
{
	colors[0] = Fade(DARKGRAY, 0.4f);
	colors[1] = Fade(GRAY, 0.5f);
	if (!is_btn_active(id, ctx))
		return ;
	colors[0] = Fade(BLACK, 0.6f);
	colors[1] = WHITE;
	if (!CheckCollisionPointRec(GetMousePosition(), btn_rect))
		return ;
	colors[0] = Fade(LIGHTGRAY, 0.4f);
	colors[1] = YELLOW;
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		handle_click(id, ctx);
}

static void	draw_sys_btn(int id, Rectangle panel, t_ui_context *ctx,
				int font_size)
{
	const char	*labels[18] = {"LOOK", "NORTH", "WEST", "EAST", "SOUTH",
		"TAKE", "DROP", "TALK", "QUEST", "ATTACK", "DEFEND", "FLEE",
		"INVENTORY", "STATUS", "QUEST LOG", "WHO", "GROUP", "QUIT"};
	Rectangle	btn_rect;
	Color		colors[2];
	int			text_width;

	btn_rect = get_btn_rect(id, panel);
	text_width = MeasureText(labels[id], font_size);
	update_btn_colors(id, btn_rect, ctx, colors);
	DrawRectangleRec(btn_rect, colors[0]);
	DrawRectangleLinesEx(btn_rect, 1.0f, LIGHTGRAY);
	DrawText(labels[id], btn_rect.x + (btn_rect.width - text_width) / 2,
		btn_rect.y + (btn_rect.height - font_size) / 2, font_size,
		colors[1]);
}

static void	draw_dividers(Rectangle panel, float row_height)
{
	float	top_y;
	float	bottom_y;

	top_y = panel.y + 35 + row_height * 4.2f;
	bottom_y = panel.y + 35 + row_height * 8.2f;
	DrawLineEx((Vector2){panel.x + 20, top_y},
		(Vector2){panel.x + panel.width - 20, top_y}, 2.0f,
		Fade(LIGHTGRAY, 1.0f));
	DrawLineEx((Vector2){panel.x + 20, bottom_y},
		(Vector2){panel.x + panel.width - 20, bottom_y}, 2.0f,
		Fade(LIGHTGRAY, 1.0f));
}

void	render_action_panel(Rectangle inv_panel, t_ui_context *ctx)
{
	Rectangle	panel;
	float		row_height;
	float		font_size;
	int			btn_id;

	panel.width = inv_panel.width;
	panel.height = inv_panel.height * 0.68f;
	panel.x = inv_panel.x;
	panel.y = inv_panel.y + (inv_panel.height - panel.height);
	DrawRectangleRec(panel, Fade(DARKGRAY, 0.8f));
	DrawRectangleLinesEx(panel, 2.0f, LIGHTGRAY);
	DrawText("Command Matrix", (int)panel.x + 15, (int)panel.y + 10, 20,
		WHITE);
	row_height = (panel.height - 45) / 11.0f;
	draw_dividers(panel, row_height);
	font_size = (row_height - 4) * 0.55f;
	if (font_size > 14)
		font_size = 14;
	btn_id = -1;
	while (++btn_id < 18)
		draw_sys_btn(btn_id, panel, ctx, font_size);
}
