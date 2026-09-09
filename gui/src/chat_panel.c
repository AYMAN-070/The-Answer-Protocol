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
	int	i;
	int	start;
	int	max_lines;

	max_lines = ((int)bounds.height - 45) / 20;
	start = 0;
	if (buffer->count > max_lines)
		start = buffer->count - max_lines;
	i = 0;
	while (start < buffer->count && i < max_lines)
	{
		DrawText(buffer->messages[start], (int)bounds.x + 15,
			(int)bounds.y + 35 + (i * 20), 16, text_color);
		start++;
		i++;
	}
}

void	add_message(t_message_buffer *buffer, const char *msg)
{
	int	i;

	if (buffer->count < 15)
	{
		strncpy(buffer->messages[buffer->count], msg, 127);
		buffer->messages[buffer->count][127] = '\0';
		buffer->count++;
	}
	else
	{
		i = 1;
		while (i < 15)
		{
			strncpy(buffer->messages[i - 1], buffer->messages[i], 128);
			i++;
		}
		strncpy(buffer->messages[14], msg, 127);
		buffer->messages[14][127] = '\0';
	}
}
