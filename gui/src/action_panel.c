/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_panel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 11:18:36 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 13:32:57 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "action_panel.h"
#include "network.h"
#include "layout.h"
#include <string.h>

static const char	*g_lbls[15] = {
	"LOOK", "NORTH", "WEST", "EAST", "SOUTH",
	"TAKE", "DROP", "TALK", "INVENTORY", "ATTACK",
	"STATUS", "QUESTS", "WHO", "GROUP", "QUIT"
};

static const char	*g_cmds[15] = {
	"LOOK\n", "MOVE north\n", "MOVE west\n", "MOVE east\n",
	"MOVE south\n", "TAKE\n", "DROP\n", "TALK\n", "INVENTORY\n",
	"ATTACK\n", "STATUS\n", "QUESTS\n", "WHO\n", "GROUP\n", "QUIT\n"
};

static int	is_btn_active(int id, t_room_state *st)
{
	int	j;

	if (id == 5)
		return (st->item_count > 0);
	if (id == 7 || id == 9)
		return (st->npc_count > 0);
	if (id >= 1 && id <= 4)
	{
		j = -1;
		while (++j < st->exit_count)
		{
			if (id == 1 && !strcmp(st->exits[j], "north")) return (1);
			if (id == 2 && !strcmp(st->exits[j], "west")) return (1);
			if (id == 3 && !strcmp(st->exits[j], "east")) return (1);
			if (id == 4 && !strcmp(st->exits[j], "south")) return (1);
		}
		return (0);
	}
	return (1);
}

static Rectangle	get_btn_rect(int i, Rectangle p)
{
	Rectangle	r;
	float		rh;

	rh = (p.height - 40) / 8.0f;
	r.height = rh - 4;
	r.width = (i >= 1 && i <= 4) ? 50 : (p.width - 50) / 3;
	if (i == 0 || i == 14)
		r.width = p.width - 30;
	r.x = p.x + 15;
	if (i >= 1 && i <= 4)
		r.x = p.x + p.width / 2 - 25 + (i == 2 ? -55 : (i == 3 ? 55 : 0));
	else if (i >= 5 && i <= 13)
		r.x = p.x + 15 + ((i - 5) % 3) * (r.width + 10);
	r.y = p.y + 35;
	if (i >= 1 && i <= 4)
		r.y += rh * (i == 1 ? 1 : (i == 4 ? 3 : 2));
	else if (i >= 5 && i <= 13)
		r.y += rh * (4 + (i - 5) / 3);
	else if (i == 14)
		r.y += rh * 7;
	return (r);
}

static void	handle_click(int id, t_ui_context *ctx)
{
	net_send_command(ctx->sock_fd, g_cmds[id]);
	if (id == 0 && ctx->player.known_count < 32)
	{
		strcpy(ctx->player.known_rooms[ctx->player.known_count],
			ctx->state.name);
		ctx->player.known_count++;
	}
}

static void	draw_sys_btn(int id, Rectangle p, t_ui_context *ctx)
{
	Rectangle	b;
	Color		bg;
	Color		txt;
	int			hover;
	int			f;

	b = get_btn_rect(id, p);
	bg = Fade(DARKGRAY, 0.4f);
	txt = Fade(GRAY, 0.5f);
	if (is_btn_active(id, &ctx->state))
	{
		hover = CheckCollisionPointRec(GetMousePosition(), b);
		bg = hover ? Fade(LIGHTGRAY, 0.4f) : Fade(BLACK, 0.6f);
		txt = hover ? YELLOW : WHITE;
		if (hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
			handle_click(id, ctx);
	}
	DrawRectangleRec(b, bg);
	DrawRectangleLinesEx(b, 1.0f, LIGHTGRAY);
	f = (b.height * 0.55f > 14) ? 14 : (int)(b.height * 0.55f);
	DrawText(g_lbls[id], b.x + (b.width - MeasureText(g_lbls[id], f)) / 2,
		b.y + (b.height - f) / 2, f, txt);
}

void	render_action_panel(Rectangle inv_panel, t_ui_context *ctx)
{
	Rectangle	p;
	int			i;

	p.x = inv_panel.x;
	p.y = inv_panel.y + (inv_panel.height / 2);
	p.width = inv_panel.width;
	p.height = inv_panel.height / 2;
	DrawRectangleRec(p, Fade(DARKGRAY, 0.8f));
	DrawRectangleLinesEx(p, 2.0f, LIGHTGRAY);
	DrawText("Command Matrix", (int)p.x + 15, (int)p.y + 15, 20, WHITE);
	i = -1;
	while (++i < 15)
		draw_sys_btn(i, p, ctx);
}
