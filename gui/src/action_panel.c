/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_panel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 11:18:36 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/10 14:44:05 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "action_panel.h"
#include "network.h"
#include "layout.h"
#include <string.h>

static int	is_btn_active(int id, t_room_state *st)
{
	const char	*dirs[5] = {"", "north", "west", "east", "south"};
	int			j;

	if (id == 5 && st->item_count == 0)
		return (0);
	if ((id == 7 || id == 9) && st->npc_count == 0)
		return (0);
	if (id >= 1 && id <= 4)
	{
		j = -1;
		while (++j < st->exit_count)
			if (!strcmp(st->exits[j], dirs[id]))
				return (1);
		return (0);
	}
	return (1);
}

static Rectangle	get_btn_rect(int i, Rectangle p)
{
	const int	nx[5] = {0, 0, -55, 55, 0};
	const int	ny[5] = {0, -1, 0, 0, 1};
	Rectangle	r;
	float		rh;

	rh = (p.height - 40) / 8.0f;
	r.height = rh - 4;
	r.width = (p.width - 50) / 3;
	r.x = p.x + 15 + ((i - 5) % 3) * (r.width + 10);
	r.y = p.y + 35 + rh * (4 + (i - 5) / 3);
	if (i == 0 || i == 14)
	{
		r.x = p.x + 15;
		r.width = p.width - 30;
		r.y = p.y + 35;
		if (i == 14)
			r.y += rh * 7;
	}
	else if (i >= 1 && i <= 4)
	{
		r.width = 50;
		r.x = p.x + p.width / 2 - 25 + nx[i];
		r.y = p.y + 35 + rh * (2 + ny[i]);
	}
	return (r);
}

static void	handle_click(int id, t_ui_context *ctx)
{
	const char	*cmd[15] = {"LOOK\n", "MOVE north\n", "MOVE west\n",
		"MOVE east\n", "MOVE south\n", "TAKE\n", "DROP\n", "TALK\n",
		"INVENTORY\n", "ATTACK\n", "STATUS\n", "QUESTS\n", "WHO\n",
		"GROUP\n", "QUIT\n"};
	int			i;

	net_send_command(ctx->sock_fd, cmd[id]);
	if (id == 0 && ctx->state.name[0] != '\0')
	{
		i = -1;
		while (++i < ctx->player.known_count)
			if (!strcmp(ctx->player.known_rooms[i], ctx->state.name))
				return ;
		if (ctx->player.known_count < 31)
			strcpy(ctx->player.known_rooms[ctx->player.known_count++],
				ctx->state.name);
	}
	else if (id == 0)
		strcpy(ctx->player.known_rooms[31], "PENDING_LOOK");
}

static void	draw_sys_btn(int id, Rectangle p, t_ui_context *ctx, int f)
{
	const char	*lbl[15] = {"LOOK", "NORTH", "WEST", "EAST", "SOUTH",
		"TAKE", "DROP", "TALK", "INVENTORY", "ATTACK", "STATUS",
		"QUESTS", "WHO", "GROUP", "QUIT"};
	Rectangle	b;
	Color		c[2];

	b = get_btn_rect(id, p);
	c[0] = Fade(DARKGRAY, 0.4f);
	c[1] = Fade(GRAY, 0.5f);
	if (is_btn_active(id, &ctx->state))
	{
		c[0] = Fade(BLACK, 0.6f);
		c[1] = WHITE;
		if (CheckCollisionPointRec(GetMousePosition(), b))
		{
			c[0] = Fade(LIGHTGRAY, 0.4f);
			c[1] = YELLOW;
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
				handle_click(id, ctx);
		}
	}
	DrawRectangleRec(b, c[0]);
	DrawRectangleLinesEx(b, 1.0f, LIGHTGRAY);
	DrawText(lbl[id], b.x + (b.width - MeasureText(lbl[id], f)) / 2,
		b.y + (b.height - f) / 2, f, c[1]);
}

void	render_action_panel(Rectangle inv_panel, t_ui_context *ctx)
{
	Rectangle	p;
	int			i;
	int			f;

	p.x = inv_panel.x;
	p.y = inv_panel.y + (inv_panel.height / 2);
	p.width = inv_panel.width;
	p.height = inv_panel.height / 2;
	DrawRectangleRec(p, Fade(DARKGRAY, 0.8f));
	DrawRectangleLinesEx(p, 2.0f, LIGHTGRAY);
	DrawText("Command Matrix", (int)p.x + 15, (int)p.y + 15, 20, WHITE);
	f = ((p.height - 40) / 8.0f - 4) * 0.55f;
	if (f > 14)
		f = 14;
	i = -1;
	while (++i < 15)
		draw_sys_btn(i, p, ctx, f);
}
