/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chat_panel.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 10:25:22 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 09:45:24 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chat_panel.h"
#include "raylib.h"
#include <string.h>


void	render_message_panel(Rectangle bounds, t_message_buffer *buffer,
			Color text_color)
{
	int	i;
	int	y_offset;

	i = 0;
	while (i < buffer->count && i < 15)
	{
		y_offset = (int)bounds.y + 35 + (i * 20);
		DrawText(buffer->messages[i], (int)bounds.x + 15, y_offset,
			16, text_color);
		i++;
	}
}

void	add_message(t_message_buffer *buffer, const char *msg)
{
	int	i;

	if (buffer->count < 15)
	{
		strncpy(buffer->messages[buffer->count], msg, 127);
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
	}
}