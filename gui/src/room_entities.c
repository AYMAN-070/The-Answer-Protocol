/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 09:03:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/11 14:54:46 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "layout.h"
#include "graphics.h"
#include <string.h>

static Vector3	get_layout(const char *id)
{
	const t_npc_layout	table[5] = {
	{"sec_drone", 0.6f, 0.6f, 0.65f},
	{"alpha_drone", 0.7f, 0.5f, 0.55f},
	{"injured_engineer", 1.0f, 0.65f, 0.70f},
	{"ai_terminal", 1.0f, 0.5f, 0.4f},
	{"cleaning_bot", 0.3f, 0.65f, 0.65f}};
	Vector3				layout;
	int					idx;

	layout.x = 0.45f;
	layout.y = 0.5f;
	layout.z = 0.5f;
	idx = -1;
	while (++idx < 5)
	{
		if (!strcmp(id, table[idx].id))
		{
			layout.x = table[idx].scale;
			layout.y = table[idx].pos_x_ratio;
			layout.z = table[idx].pos_y_ratio;
			return (layout);
		}
	}
	return (layout);
}

static Rectangle	get_npc_dest(Rectangle view_rect, Texture2D tex,
				const char *id)
{
	Vector3		layout;
	Rectangle	dest;
	float		scale;

	layout = get_layout(id);
	scale = ((view_rect.height * 0.5f) / (float)tex.height) * layout.x;
	dest.width = (float)tex.width * scale;
	dest.height = (float)tex.height * scale;
	dest.x = view_rect.x + (view_rect.width * layout.y) - (dest.width / 2);
	dest.y = view_rect.y + (view_rect.height * layout.z) - (dest.height / 2);
	return (dest);
}

void	draw_room_npcs(const t_game_assets *assets, t_ui_context *ctx,
			Rectangle view_rect)
{
	int			idx;
	Texture2D	tex;
	Rectangle	src;
	Rectangle	dest;
	Vector2		origin;

	origin.x = 0.0f;
	origin.y = 0.0f;
	idx = -1;
	while (++idx < ctx->state.npc_count)
	{
		tex = get_npc_texture(assets, ctx->state.npcs[idx].id);
		if (tex.id != 0)
		{
			src.x = 0.0f;
			src.y = 0.0f;
			src.width = (float)tex.width;
			src.height = (float)tex.height;
			dest = get_npc_dest(view_rect, tex, ctx->state.npcs[idx].id);
			DrawTexturePro(tex, src, dest, origin, 0.0f, WHITE);
		}
	}
}
