/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rfc_room.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 09:00:39 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/16 09:00:54 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rfc.h"
#include "network_parser.h"
#include <string.h>

static void	apply_id_list(cJSON *arr, t_entity *dest, int *count, cJSON *defs)
{
	cJSON	*node;

	*count = 0;
	if (!arr || !cJSON_IsArray(arr))
		return ;
	node = arr->child;
	while (node && *count < 10)
	{
		if (cJSON_IsString(node))
		{
			strncpy(dest[*count].id, node->valuestring, 31);
			dest[*count].id[31] = '\0';
			resolve_display_name(defs, node->valuestring, dest[*count].name);
			(*count)++;
		}
		node = node->next;
	}
}

void	rfc_apply_room(cJSON *root, t_ui_context *ctx)
{
	cJSON	*room;
	cJSON	*field;
	cJSON	*world;

	room = cJSON_GetObjectItem(root, "room");
	world = cJSON_GetObjectItem(ctx->world_db, "world");
	memset(&ctx->state, 0, sizeof(t_room_state));
	field = cJSON_GetObjectItem(room, "id");
	if (field && cJSON_IsString(field))
		strncpy(ctx->state.key, field->valuestring, 31);
	field = cJSON_GetObjectItem(room, "name");
	if (field && cJSON_IsString(field))
		strncpy(ctx->state.name, field->valuestring, 63);
	field = cJSON_GetObjectItem(room, "description");
	if (field && cJSON_IsString(field))
		strncpy(ctx->state.description, field->valuestring, 255);
	parse_exits(cJSON_GetObjectItem(room, "exits"), &ctx->state);
	apply_id_list(cJSON_GetObjectItem(root, "items"), ctx->state.items,
		&ctx->state.item_count, cJSON_GetObjectItem(world, "items"));
	apply_id_list(cJSON_GetObjectItem(root, "npcs"), ctx->state.npcs,
		&ctx->state.npc_count, cJSON_GetObjectItem(world, "npcs"));
	field = cJSON_GetObjectItem(root, "players");
	if (field && cJSON_IsArray(field))
		ctx->room_players = cJSON_GetArraySize(field);
}

void	rfc_apply_json(char *json, t_ui_context *ctx)
{
	cJSON	*root;

	root = cJSON_Parse(json);
	if (!root)
		return ;
	if (cJSON_GetObjectItem(root, "room"))
		rfc_apply_room(root, ctx);
	else if (cJSON_GetObjectItem(root, "attacker_hp"))
		rfc_apply_combat(root, ctx);
	else if (cJSON_GetObjectItem(root, "quest_id"))
		rfc_apply_quest(root, ctx);
	else if (cJSON_GetObjectItem(root, "hp"))
		rfc_apply_status(root, ctx);
	else if (cJSON_IsArray(root))
		rfc_apply_list(root, ctx);
	cJSON_Delete(root);
}
