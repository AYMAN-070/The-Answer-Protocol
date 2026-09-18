/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_panel_npc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 00:00:00 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/18 00:00:00 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "action_panel.h"
#include "network.h"
#include "layout.h"
#include <string.h>
#include <stdio.h>

void	apply_npc_side_effects(t_ui_context *ctx, int id)
{
	char	cmd[64];

	if (ctx->state.npc_count == 0)
		return ;
	if (id == 8)
		strncpy(ctx->attack_target, ctx->state.npcs[ctx->sel_npc].id, 31);
	else if (id == 7)
	{
		strncpy(ctx->talk_npc, ctx->state.npcs[ctx->sel_npc].name, 31);
		if (is_quest_giver(ctx, ctx->state.npcs[ctx->sel_npc].id))
		{
			snprintf(cmd, sizeof(cmd), "QUEST %s\n",
				ctx->state.npcs[ctx->sel_npc].id);
			net_send_command(ctx->sock_fd, cmd);
		}
	}
}
