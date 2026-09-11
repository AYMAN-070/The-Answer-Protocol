/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_entities.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 09:03:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/11 10:05:27 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "layout.h"
#include "graphics.h"
#include <string.h>

/*
** Retrieves custom layout data for a given NPC ID.
** Vector3 mapping: x = scale, y = pos_x (ratio), z = pos_y (ratio).
*/
static Vector3	get_layout(const char *id)
{
	const char	*ids[5] = {"sec_drone", "alpha_drone", "injured_engineer",
		"ai_terminal", "cleaning_bot"};
	const float	sc[5] = {0.6f, 0.7f, 1.0f, 1.0f, 0.35f};
	const float	px[5] = {0.6f, 0.5f, 0.65f, 0.5f, 0.65f};
	const float	py[5] = {0.65f, 0.55f, 0.70f, 0.4f, 0.65f};
	Vector3		v;
	int			i;

	v.x = 0.45f;
	v.y = 0.5f;
	v.z = 0.5f;
	i = -1;
	while (++i < 5)
	{
		if (!strcmp(id, ids[i]))
		{
			v.x = sc[i];
			v.y = px[i];
			v.z = py[i];
			return (v);
		}
	}
	return (v);
}

static Rectangle	get_npc_dest(Rectangle v, Texture2D t, const char *id)
{
	Vector3		l;
	Rectangle	d;
	float		scale;

	l = get_layout(id);
	scale = ((v.height * 0.5f) / (float)t.height) * l.x;
	d.width = (float)t.width * scale;
	d.height = (float)t.height * scale;
	d.x = v.x + (v.width * l.y) - (d.width / 2);
	d.y = v.y + (v.height * l.z) - (d.height / 2);
	return (d);
}

void	draw_room_npcs(const t_game_assets *assets, t_ui_context *ctx,
			Rectangle v)
{
	int			i;
	Texture2D	tex;
	Rectangle	src;
	Rectangle	dest;
	Vector2		origin;

	origin.x = 0.0f;
	origin.y = 0.0f;
	i = -1;
	while (++i < ctx->state.npc_count)
	{
		tex = get_npc_texture(assets, ctx->state.npcs[i].id);
		if (tex.id != 0)
		{
			src.x = 0.0f;
			src.y = 0.0f;
			src.width = (float)tex.width;
			src.height = (float)tex.height;
			dest = get_npc_dest(v, tex, ctx->state.npcs[i].id);
			DrawTexturePro(tex, src, dest, origin, 0.0f, WHITE);
		}
	}
}
