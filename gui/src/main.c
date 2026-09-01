/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qdecross <qdecross@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/31 09:21:28 by qdecross          #+#    #+#             */
/*   Updated: 2026/08/31 09:21:41 by qdecross         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "raylib.h"
#include "cJSON.h"
#include <stdio.h>

/**
 * @brief Initializes the graphical window and runs the main application loop.
 * 
 * This function sets up a basic Raylib window to verify that the local 
 * dependencies (Raylib and cJSON) are correctly linked and functioning.
 * It creates a 800x600 canvas and displays a welcome message until the user 
 * closes the window.
 * 
 * @return int Returns 0 upon successful execution and clean exit.
 */
int main(void) {
    const int screen_width = 800;
    const int screen_height = 600;

    InitWindow(screen_width, screen_height, "Prometheus Station - GUI Client");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        
        ClearBackground(BLACK);
        
        DrawText("Connection to Prometheus Station...", 190, 200, 20, GREEN);
        DrawText("Raylib is successfully installed!", 230, 250, 20, LIGHTGRAY);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}