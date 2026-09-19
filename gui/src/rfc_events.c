/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rfc_events.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 00:00:00 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/18 00:00:00 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rfc.h"
#include "network.h"
#include "action_panel.h"
#include "chat_panel.h"
#include <string.h>

void	handle_item_event(t_ui_context *ctx, char *data)
{
	if (!strncmp(data, "taken=", 6))
		update_room_items(ctx, data + 6, 1);
	else
		update_room_items(ctx, data + 8, 0);
	net_send_command(ctx->sock_fd, "INVENTORY\n");
}

void	handle_talk_response(char *data, t_ui_context *ctx)
{
	strncpy(ctx->state.current_npc, ctx->talk_npc, 31);
	strncpy(ctx->state.current_dialogue, data, 255);
	add_message(&ctx->chat, data);
	if (is_quest_giver(ctx, ctx->talk_npc_id))
	{
		net_send_command(ctx->sock_fd, "INVENTORY\n");
		net_send_command(ctx->sock_fd, "QUESTS\n");
	}
}
