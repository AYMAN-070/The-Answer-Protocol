/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chat_panel_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 16:00:00 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/11 16:00:00 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chat_panel.h"
#include "layout.h"
#include "network.h"
#include "raylib.h"
#include <string.h>
#include <stdio.h>

static void	capture_chat_chars(t_ui_context *ctx)
{
	int		key;
	size_t	len;

	len = strlen(ctx->chat_input);
	key = GetCharPressed();
	while (key > 0)
	{
		if (key >= 32 && key < 127 && len < sizeof(ctx->chat_input) - 1)
			ctx->chat_input[len++] = (char)key;
		key = GetCharPressed();
	}
	ctx->chat_input[len] = '\0';
	if (IsKeyPressed(KEY_BACKSPACE) && len > 0)
		ctx->chat_input[len - 1] = '\0';
}

static int	send_group_cmd(t_ui_context *ctx)
{
	char	cmd[192];

	if (ctx->chat_input[0] != '/')
		return (0);
	if (!strncmp(ctx->chat_input + 1, "create", 6))
		snprintf(cmd, sizeof(cmd), "GROUP CREATE\n");
	else if (!strncmp(ctx->chat_input + 1, "leave", 5))
		snprintf(cmd, sizeof(cmd), "GROUP LEAVE\n");
	else if (!strncmp(ctx->chat_input + 1, "invite ", 7))
		snprintf(cmd, sizeof(cmd), "GROUP INVITE %s\n", ctx->chat_input + 8);
	else if (!strncmp(ctx->chat_input + 1, "join ", 5))
		snprintf(cmd, sizeof(cmd), "GROUP JOIN %s\n", ctx->chat_input + 6);
	else
		return (0);
	strncpy(ctx->last_cmd, "GROUP", 15);
	net_send_command(ctx->sock_fd, cmd);
	add_message(&ctx->group_chat, ctx->chat_input);
	ctx->chat_input[0] = '\0';
	return (1);
}

static void	send_chat_message(t_ui_context *ctx)
{
	const char			*scope;
	char				cmd[192];
	char				echo[192];
	t_message_buffer	*target;

	if (ctx->chat_channel == 2 && send_group_cmd(ctx))
		return ;
	scope = "ROOM";
	target = &ctx->chat;
	if (ctx->chat_channel == 1)
	{
		scope = "GLOBAL";
		target = &ctx->global_chat;
	}
	else if (ctx->chat_channel == 2)
	{
		scope = "GROUP";
		target = &ctx->group_chat;
	}
	snprintf(cmd, sizeof(cmd), "CHAT %s %s\n", scope, ctx->chat_input);
	strncpy(ctx->last_cmd, "CHAT", 15);
	net_send_command(ctx->sock_fd, cmd);
	snprintf(echo, sizeof(echo), "You: %s", ctx->chat_input);
	add_message(target, echo);
	ctx->chat_input[0] = '\0';
}

void	update_chat_input(t_ui_context *ctx)
{
	if (IsKeyPressed(KEY_TAB))
		ctx->chat_channel = (ctx->chat_channel + 1) % 3;
	capture_chat_chars(ctx);
	if (IsKeyPressed(KEY_ENTER) && ctx->chat_input[0] != '\0')
		send_chat_message(ctx);
}
