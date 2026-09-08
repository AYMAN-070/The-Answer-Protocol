/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   room_view.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:27:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/03 09:55:01 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROOM_VIEW_H
# define ROOM_VIEW_H

# include "raylib.h"
# include "graphics.h"
# include "state_parser.h"

void	draw_room_view(const t_game_assets *assets, t_room_state *state,
			Rectangle room_panel);

#endif
