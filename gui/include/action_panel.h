/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_panel.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 11:18:17 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/03 08:00:26 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACTION_PANEL_H
# define ACTION_PANEL_H

# include "raylib.h"

typedef struct s_action_btn
{
	Rectangle	bounds;
	char		label[32];
	char		command[64];
	int			active;
}	t_action_btn;

typedef struct s_action_list
{
	t_action_btn	buttons[10];
	int				count;
}	t_action_list;

void	render_action_panel(Rectangle inv_panel, t_action_list *list);

#endif