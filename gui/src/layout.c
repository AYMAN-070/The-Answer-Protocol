#include "raylib.h"
#include "layout.h"

static void draw_ui_panel(int x, int y, int width, int height, const char* title) {
    DrawRectangle(x, y, width, height, DARKGRAY);
    DrawRectangleLines(x, y, width, height, LIGHTGRAY);
    DrawText(title, x + 10, y + 10, 20, WHITE);
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

    // Draw base panels
    draw_ui_panel(padding, padding, room_width, top_height, "Room View");
    draw_ui_panel(inv_x, padding, inv_width, top_height, "Inventory & Status");
    draw_ui_panel(padding, chat_y, sw - (padding * 2), chat_height, "Chat & Action Logs");

    // Anchor minimap to the bottom-left of the Room View panel (with a 10px internal margin)
    int map_anchor_x = padding + 10;
    int map_anchor_y = padding + top_height - 10;
    render_minimap(map_anchor_x, map_anchor_y);
}