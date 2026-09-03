/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_panel.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 09:39:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/03 09:51:30 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATUS_PANEL_H
# define STATUS_PANEL_H

# include "raylib.h"
# include "state_parser.h"
# include "graphics.h"

void	render_status_panel(Rectangle bounds, t_player_state *player,
			const t_game_assets *assets);

#endif