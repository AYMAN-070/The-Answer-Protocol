/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layout.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:08:08 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 13:25:50 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LAYOUT_H
# define LAYOUT_H

# include "raylib.h"
# include "graphics.h"
# include "chat_panel.h"
# include "cJSON.h"

typedef struct s_entity
{
	char	id[32];
	char	name[32];
}	t_entity;

typedef struct s_room_state
{
	char		name[64];
	char		description[256];
	char		exits[4][16];
	int			exit_count;
	t_entity	items[10];
	int			item_count;
	t_entity	npcs[10];
	int			npc_count;
	char		current_npc[32];
	char		current_dialogue[256];
}	t_room_state;

typedef struct s_player_state
{
	t_entity	inventory[10];
	int			inv_count;
	int			hp;
	int			max_hp;
	char		known_rooms[32][64];
	int			known_count;
}	t_player_state;

typedef struct s_hud_layout
{
	Rectangle	room;
	Rectangle	inventory;
	Rectangle	chat;
	Rectangle	logs;
}	t_hud_layout;

typedef struct s_ui_context
{
	int					sock_fd;
	t_message_buffer	chat;
	t_message_buffer	global_chat;
	t_message_buffer	group_chat;
	t_message_buffer	logs;
	char				chat_input[128];
	int					chat_channel;
	char				talk_npc[32];
	char				talk_npc_id[32];
	char				attack_target[32];
	char				last_cmd[16];
	char				last_inv_sig[320];
	char				last_quest_sig[320];
	char				group_id[32];
	int					sel_item;
	int					sel_room_item;
	int					sel_npc;
	int					connected;
	char				rx_buf[4096];
	int					room_players;
	int					server_players;
	cJSON				*world_db;
	t_room_state		state;
	t_player_state		player;
}	t_ui_context;

void	render_main_layout(const t_game_assets *assets, t_ui_context *ctx);

#endif
