/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rfc_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 09:00:39 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/16 09:01:14 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rfc.h"
#include "chat_panel.h"
#include "network.h"
#include "network_parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static void	route_presence(char *data, t_ui_context *ctx)
{
	char	msg[160];

	if (!strncmp(data, "ENTER ", 6))
	{
		ctx->room_players++;
		snprintf(msg, sizeof(msg), "%s entered the room", data + 6);
	}
	else if (!strncmp(data, "LEAVE ", 6))
	{
		if (ctx->room_players > 0)
			ctx->room_players--;
		snprintf(msg, sizeof(msg), "%s left the room", data + 6);
	}
	else
		snprintf(msg, sizeof(msg), "%s", data);
	add_message(&ctx->logs, msg);
}

static void	route_evt(char *data, t_ui_context *ctx)
{
	if (!strncmp(data, "STATS players=", 14))
		ctx->server_players = atoi(data + 14);
	else if (!strncmp(data, "ROOM CHAT ", 10))
		add_message(&ctx->chat, data + 10);
	else if (!strncmp(data, "GLOBAL CHAT ", 12))
		add_message(&ctx->global_chat, data + 12);
	else if (!strncmp(data, "GROUP CHAT ", 11))
		add_message(&ctx->group_chat, data + 11);
	else if (!strncmp(data, "GROUP ", 6))
		add_message(&ctx->group_chat, data);
	else if (!strncmp(data, "ROOM PRESENCE ", 14))
		route_presence(data + 14, ctx);
	else
		add_message(&ctx->logs, data);
}

static void	route_ok(char *data, t_ui_context *ctx)
{
	if (data[0] == '{' || data[0] == '[')
		rfc_apply_json(data, ctx);
	else if (!strncmp(data, "players=", 8))
		confirm_who(ctx, atoi(data + 8), data);
	else if (!strncmp(data, "group=", 6))
	{
		snprintf(ctx->group_id, sizeof(ctx->group_id), "%s", data + 6);
		add_message(&ctx->logs, data);
	}
	else if (!strncmp(data, "room=", 5))
		net_send_command(ctx->sock_fd, "LOOK\n");
	else if (!strncmp(data, "taken=", 6) || !strncmp(data, "dropped=", 8))
		handle_item_event(ctx, data);
	else if (!strcmp(ctx->last_cmd, "TALK"))
	{
		strncpy(ctx->state.current_npc, ctx->talk_npc, 31);
		strncpy(ctx->state.current_dialogue, data, 255);
		add_message(&ctx->chat, data);
		net_send_command(ctx->sock_fd, "INVENTORY\n");
		net_send_command(ctx->sock_fd, "QUESTS\n");
	}
	else
		add_message(&ctx->logs, data);
}

static void	route_err(char *data, t_ui_context *ctx)
{
	char	msg[192];

	if (!strncmp(data, "201 ", 4))
		snprintf(msg, sizeof(msg), "Name already in use");
	else if (!strncmp(data, "301 ", 4))
		snprintf(msg, sizeof(msg), "No exit that way");
	else if (!strncmp(data, "401 ", 4) || !strncmp(data, "402 ", 4))
		snprintf(msg, sizeof(msg), "Group: %s", data + 4);
	else if (!strncmp(data, "404 ", 4))
		snprintf(msg, sizeof(msg), "Not found: %s", data + 4);
	else if (!strncmp(data, "405 ", 4))
		snprintf(msg, sizeof(msg), "This target cannot be attacked");
	else if (!strncmp(data, "406 ", 4))
		snprintf(msg, sizeof(msg), "No quest available");
	else
		snprintf(msg, sizeof(msg), "Error: %s", data);
	add_message(&ctx->logs, msg);
}

int	rfc_route_line(char *line, t_ui_context *ctx)
{
	if (!strcmp(line, "OK"))
		return (1);
	if (!strncmp(line, "OK ", 3))
		route_ok(line + 3, ctx);
	else if (!strncmp(line, "ERR ", 4))
		route_err(line + 4, ctx);
	else if (!strncmp(line, "EVT ", 4))
		route_evt(line + 4, ctx);
	else
		return (0);
	return (1);
}
