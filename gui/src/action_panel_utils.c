/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_panel_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 15:00:00 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/11 15:00:00 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "action_panel.h"
#include "layout.h"
#include <string.h>

int	is_btn_active(int id, t_ui_context *ctx)
{
	const char	*exit_dirs[5] = {"", "north", "west", "east", "south"};
	int			idx;
	int			room_known;

	room_known = 0;
	idx = -1;
	while (++idx < ctx->player.known_count)
		if (!strcmp(ctx->player.known_rooms[idx], ctx->state.name))
			room_known = 1;
	if (!room_known && !(id == 0 || (id >= 11 && id <= 16)))
		return (0);
	if (id == 5 && ctx->state.item_count == 0)
		return (0);
	if ((id >= 7 && id <= 10) && ctx->state.npc_count == 0)
		return (0);
	if (id >= 1 && id <= 4)
	{
		idx = -1;
		while (++idx < ctx->state.exit_count)
			if (!strcmp(ctx->state.exits[idx], exit_dirs[id]))
				return (1);
		return (0);
	}
	return (1);
}

void	remember_room(t_ui_context *ctx)
{
	int	idx;

	if (ctx->state.name[0] == '\0')
	{
		strcpy(ctx->player.known_rooms[31], "PENDING_LOOK");
		return ;
	}
	idx = -1;
	while (++idx < ctx->player.known_count)
		if (!strcmp(ctx->player.known_rooms[idx], ctx->state.name))
			return ;
	if (ctx->player.known_count < 31)
		strcpy(ctx->player.known_rooms[ctx->player.known_count++],
			ctx->state.name);
}

Rectangle	get_btn_rect(int btn_id, Rectangle panel)
{
	const int	offsets[5][2] = {{0, 0}, {0, -1}, {-55, 0}, {55, 0}, {0, 1}};
	Rectangle	rect;
	float		row_height;

	row_height = (panel.height - 45) / 9.5f;
	rect.height = row_height - 4;
	rect.width = (panel.width - 50) / 3;
	if (btn_id == 0)
	{
		rect.x = panel.x + 15;
		rect.width = panel.width - 30;
		rect.y = panel.y + 35;
	}
	else if (btn_id < 5)
	{
		rect.width = 50;
		rect.x = panel.x + panel.width / 2 - 25 + offsets[btn_id][0];
		rect.y = panel.y + 35 + row_height * (2.0f + offsets[btn_id][1]);
	}
	else
	{
		rect.x = panel.x + 15 + ((btn_id - 5) % 3) * (rect.width + 10);
		rect.y = panel.y + 35 + row_height * (4.5f + (btn_id - 5) / 3 * 1.3f);
	}
	return (rect);
}
