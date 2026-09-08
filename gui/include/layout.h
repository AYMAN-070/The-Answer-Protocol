/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layout.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:08:08 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/08 10:33:16 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LAYOUT_H
# define LAYOUT_H

# include "raylib.h"
# include "graphics.h"
# include "state_parser.h"
# include "chat_panel.h"

typedef struct s_hud_layout
{
	Rectangle	room;
	Rectangle	inventory;
	Rectangle	chat;
	Rectangle	logs;
}	t_hud_layout;

typedef struct s_ui_context
{
	t_room_state		state;
	t_player_state		player;
	t_action_list		actions;
	t_message_buffer	chat;
	t_message_buffer	logs;
	int					sock_fd;
}	t_ui_context;

void	render_main_layout(const t_game_assets *assets, t_ui_context *ctx);

#endif
