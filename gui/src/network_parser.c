/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 09:00:00 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/21 15:52:10 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network_parser.h"
#include "chat_panel.h"
#include "rfc.h"
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

void	parse_exits(cJSON *exits_obj, t_room_state *state)
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
		if (!rfc_route_line(line, ctx))
			add_message(&ctx->logs, line);
		line = newline;
	}
}
