/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 09:00:00 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/11 14:28:06 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network_parser.h"
#include "chat_panel.h"
#include <string.h>
#include <stdio.h>

static void	parse_entities(cJSON *arr, t_entity *dest, int *count)
{
	cJSON	*node;
	cJSON	*npc_type;
	char	*name;

	*count = 0;
	if (!arr || !cJSON_IsArray(arr))
		return ;
	node = arr->child;
	while (node && *count < 10)
	{
		name = NULL;
		npc_type = cJSON_GetObjectItem(node, "npc_type");
		if (cJSON_IsString(node))
			name = node->valuestring;
		else if (cJSON_IsObject(node) && npc_type && cJSON_IsString(npc_type))
			name = npc_type->valuestring;
		if (name)
		{
			strncpy(dest[*count].id, name, 31);
			strncpy(dest[*count].name, name, 31);
			(*count)++;
		}
		node = node->next;
	}
}

static void	parse_exits(cJSON *exits_obj, t_room_state *state)
{
	cJSON	*exit_node;

	state->exit_count = 0;
	if (!exits_obj || !cJSON_IsObject(exits_obj))
		return ;
	exit_node = exits_obj->child;
	while (exit_node && state->exit_count < 4)
	{
		strncpy(state->exits[state->exit_count++], exit_node->string, 15);
		exit_node = exit_node->next;
	}
}

static void	fetch_room_data(t_ui_context *ctx, char *room_name)
{
	cJSON	*world;
	cJSON	*room;
	cJSON	*field;

	world = cJSON_GetObjectItem(ctx->world_db, "world");
	room = cJSON_GetObjectItem(world, "locations")->child;
	while (room)
	{
		field = cJSON_GetObjectItem(room, "name");
		if (field && strcmp(field->valuestring, room_name) == 0)
		{
			memset(&ctx->state, 0, sizeof(t_room_state));
			strncpy(ctx->state.name, field->valuestring, 63);
			field = cJSON_GetObjectItem(room, "description");
			if (field)
				strncpy(ctx->state.description, field->valuestring, 255);
			parse_exits(cJSON_GetObjectItem(room, "exits"), &ctx->state);
			parse_entities(cJSON_GetObjectItem(room, "items"),
				ctx->state.items, &ctx->state.item_count);
			field = cJSON_GetObjectItem(room, "spawns");
			parse_entities(field, ctx->state.npcs, &ctx->state.npc_count);
			break ;
		}
		room = room->next;
	}
}

static void	parse_player_state(char *buffer, t_player_state *player)
{
	char	*token;
	int		idx;

	if (strncmp(buffer, "DEAD:", 5) == 0)
		player->hp = 50;
	if (strstr(buffer, "HP:"))
		sscanf(strstr(buffer, "HP:"), "HP:%d", &player->hp);
	if (strncmp(buffer, "INVENTORY:", 10) != 0)
		return ;
	idx = -1;
	while (++idx < 5)
		player->inventory[idx].id[0] = '\0';
	player->inv_count = 0;
	if (strstr(buffer, "empty"))
		return ;
	token = strtok(buffer + 10, ",\n");
	while (token && player->inv_count < 5)
	{
		strncpy(player->inventory[player->inv_count].id, token, 31);
		strncpy(player->inventory[player->inv_count].name, token, 31);
		player->inv_count++;
		token = strtok(NULL, ",\n");
	}
}

void	parse_server_text(char *buffer, t_ui_context *ctx)
{
	char	*line;
	char	*newline;

	line = buffer;
	while (line && *line)
	{
		newline = strchr(line, '\n');
		if (newline)
			*(newline++) = '\0';
		parse_player_state(line, &ctx->player);
		if (strstr(line, "Current Room : "))
			fetch_room_data(ctx, strstr(line, "Current Room : ") + 15);
		route_message(line, ctx);
		line = newline;
	}
}
