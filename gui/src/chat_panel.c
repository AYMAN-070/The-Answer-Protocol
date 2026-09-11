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
#include "raylib.h"
#include <string.h>

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
