/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_view.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:27:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/11 09:15:50 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOM_VIEW_H
# define ROOM_VIEW_H

# include "raylib.h"
# include "graphics.h"
# include "layout.h"

void		draw_room_view(const t_game_assets *assets, t_ui_context *ctx,
				Rectangle room_panel);
void		draw_room_npcs(const t_game_assets *assets, t_ui_context *ctx,
				Rectangle v);
void		draw_room_items(const t_game_assets *assets, t_ui_context *ctx,
				Rectangle view_rect);
Rectangle	get_npc_dest(Rectangle view_rect, Texture2D tex, const char *id);

#endif
