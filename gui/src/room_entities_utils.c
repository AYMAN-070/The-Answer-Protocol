/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_entities_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 00:00:00 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/17 00:00:00 by qdecross         ###   ########.fr       */
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
	{"alpha_drone", 0.6f, 0.36f, 0.75f},
	{"injured_engineer", 1.0f, 0.65f, 0.70f},
	{"ai_terminal", 0.3f, 0.54f, 0.59f},
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

Rectangle	get_npc_dest(Rectangle view_rect, Texture2D tex, const char *id)
{
	Vector3		layout;
	Rectangle	dest;
	float		scale;

	layout = get_layout(id);
	if (tex.id != 0)
	{
		scale = ((view_rect.height * 0.5f) / (float)tex.height) * layout.x;
		dest.width = (float)tex.width * scale;
		dest.height = (float)tex.height * scale;
	}
	else
	{
		dest.width = view_rect.width * 0.12f * (layout.x / 0.45f);
		dest.height = view_rect.height * 0.35f * (layout.x / 0.45f);
	}
	dest.x = view_rect.x + (view_rect.width * layout.y) - (dest.width / 2);
	dest.y = view_rect.y + (view_rect.height * layout.z) - (dest.height / 2);
	return (dest);
}
