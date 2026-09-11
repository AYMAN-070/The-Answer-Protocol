/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chat_panel.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 10:25:22 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 13:13:13 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chat_panel.h"
#include "layout.h"
#include "raylib.h"
#include <string.h>
#include <stdio.h>

void	render_message_panel(Rectangle bounds, t_message_buffer *buffer,
			Color text_color)
{
	int	row;
	int	start;
	int	max_lines;

	max_lines = ((int)bounds.height - 45) / 20;
	start = 0;
	if (buffer->count > max_lines)
		start = buffer->count - max_lines;
	row = 0;
	while (start < buffer->count && row < max_lines)
	{
		DrawText(buffer->messages[start], (int)bounds.x + 15,
			(int)bounds.y + 35 + (row * 20), 16, text_color);
		start++;
		row++;
	}
}

void	add_message(t_message_buffer *buffer, const char *msg)
{
	int	idx;

	if (buffer->count < 15)
	{
		strncpy(buffer->messages[buffer->count], msg, 127);
		buffer->messages[buffer->count][127] = '\0';
		buffer->count++;
	}
	else
	{
		idx = 1;
		while (idx < 15)
		{
			strncpy(buffer->messages[idx - 1], buffer->messages[idx], 128);
			idx++;
		}
		strncpy(buffer->messages[14], msg, 127);
		buffer->messages[14][127] = '\0';
	}
}

static void	draw_chat_input(Rectangle bounds, const char *input, int channel)
{
	char		display[192];
	const char	*label;

	if (channel == 0)
		label = "ROOM";
	else
		label = "GLOBAL";
	DrawRectangleRec(bounds, Fade(BLACK, 0.85f));
	DrawRectangleLinesEx(bounds, 1.0f, LIGHTGRAY);
	snprintf(display, sizeof(display), "[%s] > %s_", label, input);
	DrawText(display, (int)bounds.x + 8, (int)bounds.y + 7, 16, WHITE);
}

void	render_chat_panel(Rectangle bounds, t_ui_context *ctx)
{
	Rectangle			msg_area;
	Rectangle			input_area;
	t_message_buffer	*active;

	msg_area = bounds;
	msg_area.height = bounds.height - 30;
	input_area.x = bounds.x;
	input_area.y = bounds.y + bounds.height - 28;
	input_area.width = bounds.width;
	input_area.height = 26;
	if (ctx->chat_channel == 0)
		active = &ctx->chat;
	else
		active = &ctx->global_chat;
	render_message_panel(msg_area, active, WHITE);
	update_chat_input(ctx);
	draw_chat_input(input_area, ctx->chat_input, ctx->chat_channel);
}
