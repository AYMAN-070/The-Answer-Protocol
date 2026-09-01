/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:21:38 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/01 13:26:04 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"

void init_graphics_assets(void);

void unload_graphics_assets(void);

Texture2D get_room_texture(const char* room_name);

#endif