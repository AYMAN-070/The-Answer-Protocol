/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chat_panel.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 10:24:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 09:42:04 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHAT_PANEL_H
# define CHAT_PANEL_H

# include "raylib.h"

typedef struct s_message_buffer
{
	char	messages[15][128];
	int		count;
}	t_message_buffer;

void	render_message_panel(Rectangle bounds, t_message_buffer *buffer,
			Color text_color);
void	add_message(t_message_buffer *buffer, const char *msg);

#endif