/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rfc_payload.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 09:00:39 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/16 09:01:06 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rfc.h"
#include "network_parser.h"
#include "chat_panel.h"
#include <string.h>
#include <stdio.h>

void	rfc_apply_status(cJSON *root, t_ui_context *ctx)
{
	cJSON	*field;
	char	msg[160];

	field = cJSON_GetObjectItem(root, "hp");
	if (field && cJSON_IsNumber(field))
		ctx->player.hp = field->valueint;
	field = cJSON_GetObjectItem(root, "max_hp");
	if (field && cJSON_IsNumber(field))
		ctx->player.max_hp = field->valueint;
	field = cJSON_GetObjectItem(root, "status");
	if (field && cJSON_IsString(field))
	{
		snprintf(msg, sizeof(msg), "Status: %s", field->valuestring);
		add_message(&ctx->logs, msg);
	}
}

void	rfc_apply_combat(cJSON *root, t_ui_context *ctx)
{
	cJSON	*own_hp;
	cJSON	*target_hp;
	cJSON	*damage;
	char	msg[160];

	own_hp = cJSON_GetObjectItem(root, "attacker_hp");
	target_hp = cJSON_GetObjectItem(root, "target_hp");
	damage = cJSON_GetObjectItem(root, "damage");
	if (own_hp && cJSON_IsNumber(own_hp))
		ctx->player.hp = own_hp->valueint;
	if (own_hp && target_hp && damage)
		snprintf(msg, sizeof(msg), "Damage %d - target %d HP - you %d HP",
			damage->valueint, target_hp->valueint, own_hp->valueint);
	else
		snprintf(msg, sizeof(msg), "Combat update");
	add_message(&ctx->logs, msg);
}

void	rfc_apply_quest(cJSON *root, t_ui_context *ctx)
{
	cJSON	*quest_id;
	cJSON	*label;
	char	msg[160];

	quest_id = cJSON_GetObjectItem(root, "quest_id");
	label = cJSON_GetObjectItem(root, "description");
	if (!label)
		label = cJSON_GetObjectItem(root, "status");
	if (quest_id && label && cJSON_IsString(quest_id) && cJSON_IsString(label))
		snprintf(msg, sizeof(msg), "[%s] %s", quest_id->valuestring,
			label->valuestring);
	else if (quest_id && cJSON_IsString(quest_id))
		snprintf(msg, sizeof(msg), "Quest: %s", quest_id->valuestring);
	else
		snprintf(msg, sizeof(msg), "Quest update");
	add_message(&ctx->logs, msg);
}

static void	rfc_apply_inventory(cJSON *arr, t_ui_context *ctx)
{
	cJSON		*node;
	cJSON		*defs;
	t_entity	*slot;

	defs = cJSON_GetObjectItem(ctx->world_db, "world");
	defs = cJSON_GetObjectItem(defs, "items");
	ctx->player.inv_count = 0;
	node = arr->child;
	while (node && ctx->player.inv_count < 10)
	{
		if (cJSON_IsString(node))
		{
			slot = &ctx->player.inventory[ctx->player.inv_count];
			strncpy(slot->id, node->valuestring, 31);
			slot->id[31] = '\0';
			resolve_display_name(defs, node->valuestring, slot->name);
			ctx->player.inv_count++;
		}
		node = node->next;
	}
	confirm_inventory(ctx);
}

void	rfc_apply_list(cJSON *root, t_ui_context *ctx)
{
	cJSON	*node;

	node = root->child;
	if (!strcmp(ctx->last_cmd, "QUESTS") || (node && !cJSON_IsString(node)))
	{
		if (!node)
			add_message(&ctx->logs, "No active quest");
		while (node)
		{
			rfc_apply_quest(node, ctx);
			node = node->next;
		}
		return ;
	}
	rfc_apply_inventory(root, ctx);
}
