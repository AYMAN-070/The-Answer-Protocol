/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layout.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:27:53 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/01 13:57:05 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "layout.h"
#include "map.h"
#include "graphics.h"

static void draw_ui_panel(int x, int y, int width, int height, const char* title) {
    DrawRectangle(x, y, width, height, DARKGRAY);
    DrawRectangleLines(x, y, width, height, LIGHTGRAY);
    DrawText(title, x + 10, y + 10, 20, WHITE);
}

static void draw_room_description_overlay(const char* title, const char* description, Rectangle view_rect) {
    int box_width = view_rect.width - 100;
    int box_height = 120;
    int box_x = view_rect.x + 50;
    int box_y = view_rect.y + view_rect.height - box_height - 30;

    Rectangle bounds = { (float)box_x, (float)box_y, (float)box_width, (float)box_height };

    DrawRectangleRec(bounds, Fade(BLACK, 0.75f));
    DrawRectangleLinesEx(bounds, 2.0f, Fade(LIGHTGRAY, 0.5f));

    DrawText(title, box_x + 15, box_y + 15, 24, WHITE);
    
    DrawText(description, box_x + 15, box_y + 50, 18, LIGHTGRAY);
}

void render_main_layout(void) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    int padding = 10;
    
    int room_width = (sw * 65) / 100; 
    int top_height = (sh * 70) / 100;
    
    int inv_x = room_width + (padding * 2);
    int inv_width = sw - room_width - (padding * 3);
    
    int chat_y = top_height + (padding * 2);
    int chat_height = sh - top_height - (padding * 3);

    draw_ui_panel(padding, padding, room_width, top_height, "Room View");
    draw_ui_panel(inv_x, padding, inv_width, top_height, "Inventory & Status");
    draw_ui_panel(padding, chat_y, sw - (padding * 2), chat_height, "Chat & Action Logs");

    Texture2D current_bg = get_room_texture("Main Corridor");
    Rectangle room_view_bounds = { (float)padding + 2, (float)padding + 30, (float)room_width - 4, (float)top_height - 32 };
    
    if (current_bg.id != 0) {
        Rectangle source = { 0.0f, 0.0f, (float)current_bg.width, (float)current_bg.height };
        Vector2 origin = { 0.0f, 0.0f };
        DrawTexturePro(current_bg, source, room_view_bounds, origin, 0.0f, WHITE);
    }

    draw_room_description_overlay(
        "Main Corridor", 
        "The central artery of the station. Red emergency lights are flashing.", 
        room_view_bounds
    );

int map_width = 175;
    int map_height = 125;
    
    int map_anchor_x = padding + room_width - map_width - 15;
    
    int map_anchor_y = padding + 30 + 15 + map_height; 
    
    render_minimap(map_anchor_x, map_anchor_y);
}
