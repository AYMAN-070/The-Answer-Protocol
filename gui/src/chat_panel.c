/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chat_panel.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 10:25:22 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/08 10:25:45 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chat_panel.h"
#include "raylib.h"

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