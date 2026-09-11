/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 09:00:00 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/11 10:45:48 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network_parser.h"
#include "chat_panel.h"
#include <string.h>
#include <stdio.h>

static void	parse_entities(cJSON *arr, t_entity *dest, int *count)
{
	cJSON	*i;
	cJSON	*t;
	char	*s;

	*count = 0;
	if (!arr || !cJSON_IsArray(arr))
		return ;
	i = arr->child;
	while (i && *count < 10)
	{
		s = NULL;
		t = cJSON_GetObjectItem(i, "npc_type");
		if (cJSON_IsString(i))
			s = i->valuestring;
		else if (cJSON_IsObject(i) && t && cJSON_IsString(t))
			s = t->valuestring;
		if (s)
		{
			strncpy(dest[*count].id, s, 31);
			strncpy(dest[*count].name, s, 31);
			(*count)++;
		}
		i = i->next;
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

static void	parse_player_state(char *buffer, t_player_state *p)
{
	char	*tok;
	int		i;

	if (strncmp(buffer, "DEAD:", 5) == 0)
		p->hp = 50;
	if (strstr(buffer, "HP:"))
		sscanf(strstr(buffer, "HP:"), "HP:%d", &p->hp);
	if (strncmp(buffer, "INVENTORY:", 10) != 0)
		return ;
	i = -1;
	while (++i < 5)
		p->inventory[i].id[0] = '\0';
	p->inv_count = 0;
	if (strstr(buffer, "empty"))
		return ;
	tok = strtok(buffer + 10, ",\n");
	while (tok && p->inv_count < 5)
	{
		strncpy(p->inventory[p->inv_count].id, tok, 31);
		strncpy(p->inventory[p->inv_count].name, tok, 31);
		p->inv_count++;
		tok = strtok(NULL, ",\n");
	}
}

void	parse_server_text(char *buffer, t_ui_context *ctx)
{
	char	r_name[64];
	char	*r_ptr;

	parse_player_state(buffer, &ctx->player);
	r_ptr = strstr(buffer, "Current Room : ");
	if (r_ptr)
	{
		strncpy(r_name, r_ptr + 15, 63);
		r_name[strcspn(r_name, "\n")] = 0;
		fetch_room_data(ctx, r_name);
	}
	if (strncmp(buffer, "NPC:", 4) == 0)
		add_message(&ctx->chat, buffer + 4);
	else if (strncmp(buffer, "COMBAT:", 7) == 0)
		add_message(&ctx->logs, buffer + 7);
	else if (strncmp(buffer, "EVENT:", 6) == 0)
		add_message(&ctx->logs, buffer + 6);
	else if (strncmp(buffer, "DEAD:", 5) == 0)
		add_message(&ctx->logs, buffer + 5);
	else if (strncmp(buffer, "INVENTORY:", 10) != 0
		&& strncmp(buffer, "STATUS:", 7) != 0)
		add_message(&ctx->logs, buffer);
}
