/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 10:18:03 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/01 10:30:07 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "map.h"

static void draw_minimap_node(int x, int y, int is_current) {
    Color node_color = is_current ? GREEN : DARKGRAY;
    Color border_color = is_current ? LIGHTGRAY : GRAY;
    
    // Scale reduced to 20x10 for overlay sizing
    DrawRectangle(x - 10, y - 5, 20, 10, node_color);
    DrawRectangleLines(x - 10, y - 5, 20, 10, border_color);
}

void render_minimap(int bottom_left_x, int bottom_left_y) {
    int spacing = 35; 
    // Increased width to accommodate the western AI Core branch
    int map_width = spacing * 5; 
    int map_height = spacing * 3 + 20; 
    
    // HUD Overlay background
    int start_y = bottom_left_y - map_height;
    DrawRectangle(bottom_left_x, start_y, map_width, map_height, Fade(BLACK, 0.8f));
    DrawRectangleLines(bottom_left_x, start_y, map_width, map_height, Fade(LIGHTGRAY, 0.5f));
    DrawText("MINIMAP", bottom_left_x + 5, start_y + 5, 10, Fade(WHITE, 0.6f));

    // Calculate dynamic center, shifted right (+17) to center the asymmetrical graph
    int cx = bottom_left_x + (map_width / 2) + 17;
    int cy = start_y + (map_height / 2) + 10; 

    // Connections (Drawn first to appear under nodes)
    DrawLine(cx, cy, cx, cy + spacing, GRAY);
    DrawLine(cx, cy, cx + spacing, cy, GRAY);
    DrawLine(cx + spacing, cy, cx + spacing, cy - spacing, GRAY);
    DrawLine(cx + spacing, cy - spacing, cx, cy - spacing, GRAY);
    DrawLine(cx, cy - spacing, cx, cy, GRAY);
    DrawLine(cx, cy, cx - spacing, cy, GRAY);
    DrawLine(cx - spacing, cy, cx - (spacing * 2), cy, GRAY); // West branch to AI Core
    
    // Nodes
    draw_minimap_node(cx, cy + spacing, 0);           // Airlock
    draw_minimap_node(cx, cy, 1);                     // Corridor (Current)
    draw_minimap_node(cx + spacing, cy, 0);           // Quarters
    draw_minimap_node(cx + spacing, cy - spacing, 0); // Cafeteria
    draw_minimap_node(cx, cy - spacing, 0);           // Medbay
    draw_minimap_node(cx, cy - (spacing/2), 0);       // Armory
    draw_minimap_node(cx - spacing, cy, 0);           // Maint. Bay
    draw_minimap_node(cx - (spacing * 2), cy, 0);     // AI Core
}