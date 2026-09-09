/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 09:00:00 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 09:24:51 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network_parser.h"
#include "chat_panel.h"
#include <string.h>

static void	add_entity(t_entity *dest, int *count, char *value)
{
	strncpy(dest[*count].id, value, 31);
	strncpy(dest[*count].name, value, 31);
	(*count)++;
}

static void	parse_entities(cJSON *arr, t_entity *dest, int *count)
{
	cJSON	*item;
	cJSON	*type;

	*count = 0;
	if (!arr || !cJSON_IsArray(arr))
		return ;
	item = arr->child;
	while (item && *count < 10)
	{
		if (cJSON_IsString(item))
			add_entity(dest, count, item->valuestring);
		else if (cJSON_IsObject(item))
		{
			type = cJSON_GetObjectItem(item, "npc_type");
			if (type && cJSON_IsString(type))
				add_entity(dest, count, type->valuestring);
		}
		item = item->next;
	}
}

static void	parse_exits(cJSON *exits_obj, t_room_state *state)
{
	cJSON	*ext;

	state->exit_count = 0;
	if (!exits_obj || !cJSON_IsObject(exits_obj))
		return ;
	ext = exits_obj->child;
	while (ext && state->exit_count < 4)
	{
		strncpy(state->exits[state->exit_count++], ext->string, 15);
		ext = ext->next;
	}
}

static void	fetch_room_data(t_ui_context *ctx, char *room_name)
{
	cJSON	*world;
	cJSON	*room;
	cJSON	*tmp;

	world = cJSON_GetObjectItem(ctx->world_db, "world");
	room = cJSON_GetObjectItem(world, "locations")->child;
	while (room)
	{
		tmp = cJSON_GetObjectItem(room, "name");
		if (tmp && strcmp(tmp->valuestring, room_name) == 0)
		{
			memset(&ctx->state, 0, sizeof(t_room_state));
			strncpy(ctx->state.name, tmp->valuestring, 63);
			tmp = cJSON_GetObjectItem(room, "description");
			if (tmp)
				strncpy(ctx->state.description, tmp->valuestring, 255);
			parse_exits(cJSON_GetObjectItem(room, "exits"), &ctx->state);
			parse_entities(cJSON_GetObjectItem(room, "items"),
				ctx->state.items, &ctx->state.item_count);
			tmp = cJSON_GetObjectItem(room, "spawns");
			parse_entities(tmp, ctx->state.npcs, &ctx->state.npc_count);
			break ;
		}
		room = room->next;
	}
}

void	parse_server_text(char *buffer, t_ui_context *ctx)
{
	char	room_name[64];

	if (strncmp(buffer, "Current Room : ", 15) == 0)
	{
		strncpy(room_name, buffer + 15, 63);
		room_name[strcspn(room_name, "\n")] = 0;
		fetch_room_data(ctx, room_name);
		add_message(&ctx->logs, buffer);
	}
	else if (strncmp(buffer, "NPC:", 4) == 0)
		add_message(&ctx->chat, buffer + 4);
	else if (strncmp(buffer, "COMBAT:", 7) == 0)
		add_message(&ctx->logs, buffer + 7);
	else if (strncmp(buffer, "EVENT:", 6) == 0)
		add_message(&ctx->logs, buffer + 6);
	else
		add_message(&ctx->logs, buffer);
}
