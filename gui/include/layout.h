/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layout.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:08:08 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/01 10:08:23 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LAYOUT_H
# define LAYOUT_H

# include "raylib.h"
# include "graphics.h"

typedef struct s_hud_layout
{
	Rectangle	room;
	Rectangle	inventory;
	Rectangle	chat;
}	t_hud_layout;

void	render_main_layout(const t_room_assets *assets);

#endif
