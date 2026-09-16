/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_panel.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 09:39:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/09 11:46:16 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STATUS_PANEL_H
# define STATUS_PANEL_H

# include "raylib.h"
# include "layout.h"
# include "graphics.h"

void	render_status_panel(Rectangle bounds, t_ui_context *ctx,
			const t_game_assets *assets);

#endif