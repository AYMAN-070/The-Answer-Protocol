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
#include <string.h>

void	confirm_who(t_ui_context *ctx, int count, const char *raw)
{
	ctx->server_players = count;
	add_message(&ctx->logs, raw);
}

int	quests_changed(t_ui_context *ctx, cJSON *root)
{
	char	sig[320];
	cJSON	*node;
	cJSON	*f;

	strcpy(sig, "#");
	node = root->child;
	while (node)
	{
		f = cJSON_GetObjectItem(node, "quest_id");
		if (f && cJSON_IsString(f))
			strncat(sig, f->valuestring, sizeof(sig) - strlen(sig) - 1);
		f = cJSON_GetObjectItem(node, "status");
		if (f && cJSON_IsString(f))
			strncat(sig, f->valuestring, sizeof(sig) - strlen(sig) - 1);
		node = node->next;
	}
	if (!strcmp(sig, ctx->last_quest_sig))
		return (0);
	strncpy(ctx->last_quest_sig, sig, sizeof(ctx->last_quest_sig) - 1);
	return (1);
}

void	confirm_inventory(t_ui_context *ctx)
{
	char	sig[320];
	char	msg[64];
	int		idx;

	sig[0] = '\0';
	idx = -1;
	while (++idx < ctx->player.inv_count)
		strncat(sig, ctx->player.inventory[idx].id,
			sizeof(sig) - strlen(sig) - 1);
	if (!strcmp(sig, ctx->last_inv_sig))
		return ;
	strncpy(ctx->last_inv_sig, sig, sizeof(ctx->last_inv_sig) - 1);
	if (ctx->player.inv_count == 0)
		snprintf(msg, sizeof(msg), "Inventory: empty");
	else
		snprintf(msg, sizeof(msg), "Inventory: %d item(s)",
			ctx->player.inv_count);
	add_message(&ctx->logs, msg);
}
