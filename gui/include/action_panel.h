/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action_panel.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 11:18:17 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 11:25:48 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACTION_PANEL_H
# define ACTION_PANEL_H

# include "raylib.h"

struct s_ui_context;

void	render_action_panel(Rectangle inv_panel, struct s_ui_context *ctx);

#endif