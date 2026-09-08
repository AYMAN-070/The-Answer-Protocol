/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_parser.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 08:18:25 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/08 13:15:48 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATE_PARSER_H
# define STATE_PARSER_H

# include "action_panel.h"

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
	t_entity	items[5];
	int			item_count;
	t_entity	npcs[5];
	int			npc_count;
	int			room_players;
	int			server_players;
	char		current_npc[64];
	char		current_dialogue[256];
}	t_room_state;

typedef struct s_player_state
{
	int			hp;
	int			max_hp;
	t_entity	inventory[10];
	int			inv_count;
}	t_player_state;

void	add_action(t_action_list *list, const char *lbl, const char *cmd);
void	add_global_actions(t_action_list *list);
void	add_inventory_actions(t_action_list *list, t_player_state *player);
void	build_actions_from_state(t_action_list *list, t_room_state *state,
			t_player_state *player);

#endif