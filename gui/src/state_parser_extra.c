/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_parser_extra.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 10:00:35 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 09:52:19 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "state_parser.h"
#include <string.h>
#include <stdio.h>

void	add_global_actions(t_action_list *list)
{
	add_action(list, "STATUS", "STATUS\n");
	add_action(list, "QUESTS", "QUESTS\n");
	add_action(list, "WHO", "WHO\n");
	add_action(list, "GROUP", "GROUP\n");
	add_action(list, "QUIT", "QUIT\n");
}

void	add_inventory_actions(t_action_list *list, t_player_state *player)
{
	int		i;
	char	cmd[64];
	char	lbl[64];

	i = 0;
	while (i < player->inv_count)
	{
		snprintf(cmd, 64, "DROP %s\n", player->inventory[i].id);
		snprintf(lbl, 64, "DROP %s", player->inventory[i].name);
		add_action(list, lbl, cmd);
		i++;
	}
}
