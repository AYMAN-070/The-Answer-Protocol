/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/01 13:21:58 by qdecross          #+#    #+#             */
/*   Updated: 2026/09/01 13:36:47 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "graphics.h"
#include <string.h>
#include <stdio.h>

#define MAX_ROOMS 8

typedef struct {
    char name[64];
    Texture2D texture;
} RoomAsset;

static RoomAsset room_assets[MAX_ROOMS];
static int loaded_rooms_count = 0;

static void preload_room(const char* room_name) {
    if (loaded_rooms_count >= MAX_ROOMS) return;

    char filepath[256];
    snprintf(filepath, sizeof(filepath), "assets/pictures/rooms/%s.png", room_name);
    
    Texture2D tex = LoadTexture(filepath);
    
    strncpy(room_assets[loaded_rooms_count].name, room_name, sizeof(room_assets[loaded_rooms_count].name) - 1);
    room_assets[loaded_rooms_count].texture = tex;
    
    loaded_rooms_count++;
}

void init_graphics_assets(void) {
    loaded_rooms_count = 0;

    preload_room("Airlock");
    preload_room("Main Corridor");
    preload_room("Crew Quarters");
    preload_room("Cafeteria");
    preload_room("Medbay");
    preload_room("Armory");
    preload_room("Maintenance Bay");
    preload_room("AI Core");
}

void unload_graphics_assets(void) {
    for (int i = 0; i < loaded_rooms_count; i++) {
        UnloadTexture(room_assets[i].texture);
    }
    loaded_rooms_count = 0;
}

Texture2D get_room_texture(const char* room_name) {
    for (int i = 0; i < loaded_rooms_count; i++) {
        if (strcmp(room_assets[i].name, room_name) == 0) {
            return room_assets[i].texture;
        }
    }

    Texture2D empty = { 0 };
    return empty;
}