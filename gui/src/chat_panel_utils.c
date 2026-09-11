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

static void	send_chat_message(t_ui_context *ctx)
{
	char				cmd[192];
	char				echo[192];
	t_message_buffer	*target;

	if (ctx->chat_channel == 0)
	{
		target = &ctx->chat;
		snprintf(cmd, sizeof(cmd), "RCHAT %s\n", ctx->chat_input);
	}
	else
	{
		target = &ctx->global_chat;
		snprintf(cmd, sizeof(cmd), "GCHAT %s\n", ctx->chat_input);
	}
	net_send_command(ctx->sock_fd, cmd);
	snprintf(echo, sizeof(echo), "You: %s", ctx->chat_input);
	add_message(target, echo);
	ctx->chat_input[0] = '\0';
}

void	update_chat_input(t_ui_context *ctx)
{
	if (IsKeyPressed(KEY_TAB))
		ctx->chat_channel = !ctx->chat_channel;
	capture_chat_chars(ctx);
	if (IsKeyPressed(KEY_ENTER) && ctx->chat_input[0] != '\0')
		send_chat_message(ctx);
}
