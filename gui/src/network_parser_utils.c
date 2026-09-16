/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_parser_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 15:00:00 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/16 09:00:00 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network_parser.h"
#include "chat_panel.h"
#include <string.h>

void	resolve_display_name(cJSON *defs, const char *id, char *out)
{
	cJSON	*entry;
	cJSON	*label;

	strncpy(out, id, 31);
	out[31] = '\0';
	if (!defs)
		return ;
	entry = cJSON_GetObjectItem(defs, id);
	if (!entry)
		return ;
	label = cJSON_GetObjectItem(entry, "name");
	if (label && cJSON_IsString(label))
	{
		strncpy(out, label->valuestring, 31);
		out[31] = '\0';
	}
}

static void	parse_dropped(char *line, t_ui_context *ctx)
{
	cJSON	*defs;
	char	*token;
	int		idx;

	defs = cJSON_GetObjectItem(ctx->world_db, "world");
	defs = cJSON_GetObjectItem(defs, "items");
	token = strtok(line + 8, ",\n");
	while (token && ctx->state.item_count < 10)
	{
		idx = -1;
		while (++idx < ctx->state.item_count
			&& strcmp(ctx->state.items[idx].id, token))
			;
		if (idx == ctx->state.item_count)
		{
			strncpy(ctx->state.items[idx].id, token, 31);
			resolve_display_name(defs, token, ctx->state.items[idx].name);
			ctx->state.item_count++;
		}
		token = strtok(NULL, ",\n");
	}
}

static void	parse_who_line(char *line, t_ui_context *ctx)
{
	char	*start;
	char	*end;
	char	key[32];

	start = strstr(line, "(Salle: ");
	if (!start)
		return ;
	start += 8;
	end = strchr(start, ')');
	if (!end || end - start > 31)
		return ;
	strncpy(key, start, end - start);
	key[end - start] = '\0';
	ctx->server_players++;
	if (!strcmp(key, ctx->state.key))
		ctx->room_players++;
}

int	route_state_line(char *line, t_ui_context *ctx)
{
	if (!strncmp(line, "DROPPED:", 8))
	{
		parse_dropped(line, ctx);
		return (1);
	}
	if (!strncmp(line, "Joueurs en ligne", 16))
	{
		ctx->server_players = 0;
		ctx->room_players = 0;
		return (1);
	}
	if (line[0] == '-' && strstr(line, "(Salle: "))
	{
		parse_who_line(line, ctx);
		return (1);
	}
	return (0);
}

void	route_message(char *line, t_ui_context *ctx)
{
	if (!strncmp(line, "NPC:", 4))
	{
		strncpy(ctx->state.current_npc, ctx->talk_npc, 31);
		strncpy(ctx->state.current_dialogue, line + 4, 255);
		add_message(&ctx->chat, line + 4);
	}
	else if (!strncmp(line, "RCHAT:", 6))
		add_message(&ctx->chat, line + 6);
	else if (!strncmp(line, "GCHAT:", 6))
		add_message(&ctx->global_chat, line + 6);
	else if (!strncmp(line, "PCHAT:", 6))
		add_message(&ctx->group_chat, line + 6);
	else if (!strncmp(line, "COMBAT:", 7))
		add_message(&ctx->logs, line + 7);
	else if (!strncmp(line, "EVENT:", 6))
		add_message(&ctx->logs, line + 6);
	else if (!strncmp(line, "DEAD:", 5))
		add_message(&ctx->logs, line + 5);
	else if (strncmp(line, "INVENTORY:", 10) && strncmp(line, "STATUS:", 7))
		add_message(&ctx->logs, line);
}
