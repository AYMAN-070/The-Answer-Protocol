/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 08:18:52 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/08 09:59:58 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_parser.h"
#include <string.h>
#include <stdio.h>

void	add_action(t_action_list *list, const char *lbl, const char *cmd)
{
	int	i;

	i = list->count;
	if (i >= 32)
		return ;
	strncpy(list->buttons[i].label, lbl, 31);
	strncpy(list->buttons[i].command, cmd, 63);
	list->buttons[i].active = 1;
	list->count++;
}

static void	add_exit_actions(t_action_list *list, t_room_state *state)
{
	int		i;
	char	cmd[64];
	char	lbl[64];

	i = 0;
	while (i < state->exit_count)
	{
		snprintf(cmd, 64, "MOVE %s\n", state->exits[i]);
		snprintf(lbl, 64, "MOVE %s", state->exits[i]);
		add_action(list, lbl, cmd);
		i++;
	}
}

static void	add_item_actions(t_action_list *list, t_room_state *state)
{
	int		i;
	char	cmd[64];
	char	lbl[64];

	i = 0;
	while (i < state->item_count)
	{
		snprintf(cmd, 64, "TAKE %s\n", state->items[i].id);
		snprintf(lbl, 64, "TAKE %s", state->items[i].name);
		add_action(list, lbl, cmd);
		i++;
	}
}

static void	add_npc_actions(t_action_list *list, t_room_state *state)
{
	int		i;
	char	cmd[64];
	char	lbl[64];

	i = 0;
	while (i < state->npc_count)
	{
		snprintf(cmd, 64, "TALK %s\n", state->npcs[i].id);
		snprintf(lbl, 64, "TALK %s", state->npcs[i].name);
		add_action(list, lbl, cmd);
		snprintf(cmd, 64, "ATTACK %s\n", state->npcs[i].id);
		snprintf(lbl, 64, "ATTACK %s", state->npcs[i].name);
		add_action(list, lbl, cmd);
		i++;
	}
}

void	build_actions_from_state(t_action_list *list, t_room_state *state,
			t_player_state *player)
{
	list->count = 0;
	add_action(list, "LOOK", "LOOK\n");
	add_exit_actions(list, state);
	add_item_actions(list, state);
	add_npc_actions(list, state);
	add_inventory_actions(list, player);
	add_global_actions(list);
}
