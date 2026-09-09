/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_view.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:27:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 13:28:27 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOM_VIEW_H
# define ROOM_VIEW_H

# include "raylib.h"
# include "graphics.h"
# include "layout.h"

void draw_room_view(const t_game_assets *assets, t_ui_context *ctx, Rectangle room_panel);

#endif
