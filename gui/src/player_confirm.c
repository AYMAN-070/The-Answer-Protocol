/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_confirm.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 00:00:00 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/17 00:00:00 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network_parser.h"
#include "chat_panel.h"
#include <stdio.h>

void	confirm_status(t_ui_context *ctx)
{
	char	msg[64];

	snprintf(msg, sizeof(msg), "Status: HP %d/%d", ctx->player.hp,
		ctx->player.max_hp);
	add_message(&ctx->logs, msg);
}

void	confirm_who(t_ui_context *ctx, int count, const char *raw)
{
	ctx->server_players = count;
	add_message(&ctx->logs, raw);
}

void	confirm_inventory(t_ui_context *ctx)
{
	char	msg[64];

	if (ctx->player.inv_count == 0)
		snprintf(msg, sizeof(msg), "Inventory: empty");
	else
		snprintf(msg, sizeof(msg), "Inventory: %d item(s)",
			ctx->player.inv_count);
	add_message(&ctx->logs, msg);
}
