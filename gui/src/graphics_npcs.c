/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_npcs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 09:01:57 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/11 09:03:37 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"
#include <string.h>
#include <stdio.h>

static void	preload_npc(t_npc_assets *assets, const char *id)
{
	char	filepath[256];

	if (assets->count >= 10)
		return ;
	snprintf(filepath, sizeof(filepath), "assets/pictures/npcs/%s.png", id);
	assets->npcs[assets->count].texture = LoadTexture(filepath);
	strncpy(assets->npcs[assets->count].id, id, 31);
	assets->count++;
}

void	init_npc_assets(t_npc_assets *npcs)
{
	npcs->count = 0;
	preload_npc(npcs, "sec_drone");
	preload_npc(npcs, "alpha_drone");
	preload_npc(npcs, "injured_engineer");
	preload_npc(npcs, "ai_terminal");
	preload_npc(npcs, "cleaning_bot");
}

void	unload_npc_assets(t_npc_assets *npcs)
{
	int	i;

	i = -1;
	while (++i < npcs->count)
		UnloadTexture(npcs->npcs[i].texture);
	npcs->count = 0;
}

Texture2D	get_npc_texture(const t_game_assets *assets, const char *id)
{
	int			i;
	Texture2D	empty;

	i = -1;
	while (++i < assets->npcs.count)
		if (!strcmp(assets->npcs.npcs[i].id, id))
			return (assets->npcs.npcs[i].texture);
	memset(&empty, 0, sizeof(empty));
	return (empty);
}
