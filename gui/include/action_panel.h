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

struct	s_ui_context;

int			is_btn_active(int id, struct s_ui_context *ctx);
int			is_quest_giver(struct s_ui_context *ctx, const char *npc_id);
Rectangle	get_btn_rect(int btn_id, Rectangle panel);
void		remember_room(struct s_ui_context *ctx);
void		apply_npc_side_effects(struct s_ui_context *ctx, int id);
void		render_action_panel(Rectangle inv_panel, struct s_ui_context *ctx);

#endif